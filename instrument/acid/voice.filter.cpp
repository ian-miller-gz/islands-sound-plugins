// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float ONE = 1.0f;
constexpr Float CORNER = 120;
constexpr Float FIRST = -1;
constexpr Float GAINS[] = {20.065f, 10.355f, 6.652f, 5.174f, 4.545f,
                           4.261f,  4.128f,  4.063f, 4.031f, 4.016f};
constexpr Whole LAST = std::size(GAINS) - 1;

constexpr auto fitted(Float octave) -> Float {
  const Float place = octave - FIRST;
  if (place <= 0) return GAINS[0];
  if (place >= Float(LAST)) return GAINS[LAST];
  const Whole at = Whole(place);
  const Float part = place - Float(at);
  return GAINS[at] + part * (GAINS[at + 1] - GAINS[at]);
}

static_assert(fitted(FIRST) > fitted(Float(LAST)));

}  // namespace

void SOUND::PLUGINS::ACID::settle(
  Bassline &bassline, Float cutoff, Float resonance, Whole rate) {
  for (CORE::FILTER::Pole &pole : bassline.poles)
    CORE::FILTER::settle(pole, CORE::FILTER::LOW, cutoff, rate);
  CORE::FILTER::settle(bassline.leak, CORE::FILTER::HIGH, CORNER, rate);
  const Float octave = std::log2(CORE::FILTER::bounded(cutoff, rate) / CORNER);
  bassline.feedback = CORE::FILTER::bounded(resonance) * ::fitted(octave);
}

auto SOUND::PLUGINS::ACID::tick(Bassline &bassline, Float in) -> Float {
  Float through = ONE;
  Float free = 0;
  for (const CORE::FILTER::Pole &pole : bassline.poles) {
    through *= pole.gain;
    free = pole.gain * free + (ONE - pole.gain) * pole.state;
  }
  const Float held = bassline.leak.state;
  const Float loop = bassline.feedback * (ONE - bassline.leak.gain);
  const Float out =
    (through * (in + loop * held) + free) / (ONE + through * loop);
  Float signal = CORE::SHAPER::soft(in - loop * (out - held));
  for (CORE::FILTER::Pole &pole : bassline.poles)
    signal = CORE::FILTER::tick(pole, signal);
  CORE::FILTER::tick(bassline.leak, signal);
  return signal;
}
