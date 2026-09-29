// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float ROUND = 1.5f;
constexpr Float CUBE = 0.5f;
constexpr Float NYQUIST = 0.9f;
constexpr Float CEILING = PHASE::HALF * NYQUIST;
constexpr Float QUARTERS = 4.0f;
constexpr PHASE::Wheel QUARTER = PHASE::Wheel(PHASE::TURN / QUARTERS);

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::pace(Float hertz, Whole rate)
  -> Float {
  return rate == 0 ? 0 : hertz / Float(rate) * PHASE::TURN;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::rounded(PHASE::Wheel phase)
  -> Float {
  const Float wave = OSCILLATOR::NAIVE::triangle(phase);
  return wave * (ROUND - CUBE * wave * wave);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(
  Swept &swept, const Bend &bend, Whole rate) {
  swept.depth = bend.depth;
  swept.knee = bend.knee;
  shape(swept.fast, bend.fast, rate);
  shape(swept.slow, bend.slow, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(
  Swept &swept, Float pace) {
  swept.pace = pace;
  swept.phase = QUARTER;
  ENVELOPE::strike(swept.head, swept.fast, ONE);
  ENVELOPE::strike(swept.tail, swept.slow, ONE);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Swept &swept) -> Float {
  const Float head = ENVELOPE::tick(swept.head, swept.fast);
  const Float tail = ENVELOPE::tick(swept.tail, swept.slow);
  const Float bent =
    swept.pace * (ONE + swept.depth * head + swept.knee * tail);
  const Float step = bent < CEILING ? bent : CEILING;
  const Float out = rounded(swept.phase);
  swept.phase += PHASE::Wheel(step);
  return out;
}
