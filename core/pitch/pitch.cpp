// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "pitch.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole NARROWEST = 2;
constexpr Whole PAIR = 2;

auto lagged(Whole rate, Float hertz) -> Whole {
  return hertz <= 0 ? 0 : Whole(std::ceil(Float(rate) / hertz));
}

void take(CORE::PITCH::Detector &detector, Float sample) {
  detector.ring[detector.head] = sample;
  if (++detector.head == detector.span) detector.head = 0;
}

}  // namespace

void SOUND::PLUGINS::CORE::PITCH::build(
  Detector &detector, Whole rate, Float lowest, Float highest, Whole hop) {
  const Whole lags = ::lagged(rate, lowest);
  const Whole least = ::lagged(rate, highest);
  detector.lags = lags < NARROWEST * PAIR ? NARROWEST * PAIR : lags;
  detector.least = least < NARROWEST ? NARROWEST : least;
  if (detector.least >= detector.lags) detector.least = NARROWEST;
  detector.span = detector.lags * PAIR;
  detector.ring.assign(detector.span, 0);
  detector.samples.assign(detector.span, 0);
  detector.differences.assign(detector.lags + 1, 0);
  detector.hop = hop == 0 ? 1 : hop;
  detector.head = 0;
  detector.count = 0;
  detector.rate = rate;
  detector.estimate = {};
}

auto SOUND::PLUGINS::CORE::PITCH::feed(
  Detector &detector, const Float *samples, Whole frames) -> Flag {
  if (detector.span == 0) return false;
  Flag refreshed = false;
  for (Whole frame = 0; frame < frames; ++frame) {
    ::take(detector, samples[frame]);
    if (++detector.count < detector.hop) continue;
    detector.count = 0;
    analyse(detector);
    refreshed = true;
  }
  return refreshed;
}
