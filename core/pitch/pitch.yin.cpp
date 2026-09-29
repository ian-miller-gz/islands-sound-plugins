// SPDX-License-Identifier: AGPL-3.0-or-later
#include "pitch.hpp"

namespace {
using namespace SOUND;

constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float TWICE = 2.0f;

void gather(CORE::PITCH::Detector &detector) {
  Whole at = detector.head;
  for (Whole index = 0; index < detector.span; ++index) {
    detector.samples[index] = detector.ring[at];
    if (++at == detector.span) at = 0;
  }
}

void differ(CORE::PITCH::Detector &detector) {
  const Float *samples = detector.samples.data();
  Float running = 0;
  detector.differences[0] = UNITY;
  for (Whole lag = 1; lag <= detector.lags; ++lag) {
    Float sum = 0;
    for (Whole at = 0; at < detector.lags; ++at) {
      const Float gap = samples[at] - samples[at + lag];
      sum += gap * gap;
    }
    running += sum;
    detector.differences[lag] =
      running > 0 ? sum * Float(lag) / running : UNITY;
  }
}

auto pick(const CORE::PITCH::Detector &detector) -> Whole {
  const Float *differences = detector.differences.data();
  Whole best = detector.least;
  for (Whole lag = detector.least; lag <= detector.lags; ++lag) {
    if (differences[lag] < detector.threshold) {
      while (lag < detector.lags && differences[lag + 1] < differences[lag])
        ++lag;
      return lag;
    }
    if (differences[lag] < differences[best]) best = lag;
  }
  return best;
}

auto refine(const CORE::PITCH::Detector &detector, Whole lag) -> Float {
  if (lag <= detector.least || lag >= detector.lags) return Float(lag);
  const Float before = detector.differences[lag - 1];
  const Float at = detector.differences[lag];
  const Float after = detector.differences[lag + 1];
  const Float bend = before - TWICE * at + after;
  if (bend <= 0) return Float(lag);
  return Float(lag) + HALF * (before - after) / bend;
}

}  // namespace

void SOUND::CORE::PITCH::analyse(Detector &detector) {
  ::gather(detector);
  ::differ(detector);
  const Whole lag = ::pick(detector);
  const Float period = ::refine(detector, lag);
  const Float doubt = detector.differences[lag];
  detector.estimate.hertz = Float(detector.rate) / period;
  detector.estimate.confidence = doubt >= UNITY ? 0 : UNITY - doubt;
}
