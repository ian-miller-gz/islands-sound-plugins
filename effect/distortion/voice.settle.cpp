// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND;

constexpr CORE::SHAPER::Curve SHAPES[] = {
  CORE::SHAPER::hard, CORE::SHAPER::tube};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == DISTORTION::MODES);

auto swept(Float least, Float most, Float place) -> Float {
  return least * std::pow(most / least, place);
}

}  // namespace

void SOUND::DISTORTION::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  for (auto &strip : effect.strips) {
    CORE::FILTER::settle(strip.tight, CORE::FILTER::HIGH, TIGHT, effect.rate);
    CORE::SHAPER::settle(strip.blocker, STILL, effect.rate);
  }
}

void SOUND::DISTORTION::settle(Effect &effect) {
  const Whole mode = Whole(effect.rows[MODE]);
  effect.curve = SHAPES[mode < MODES ? mode : HARD];
  effect.gain = ::swept(GENTLEST, HOTTEST, effect.rows[DRIVE]);
  effect.level = CORE::DYNAMICS::gain(effect.rows[LEVEL]);
  const Float cutoff = ::swept(DARKEST, BRIGHTEST, effect.rows[TONE]);
  for (auto &strip : effect.strips)
    CORE::FILTER::settle(strip.tone, CORE::FILTER::LOW, cutoff, effect.rate);
}
