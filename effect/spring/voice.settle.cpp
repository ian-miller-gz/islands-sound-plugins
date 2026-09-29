// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto seconds(Float tension) -> Float {
  return SPRING::LONGEST *
         std::pow(SPRING::SHORTEST / SPRING::LONGEST, tension);
}

auto chirp(Float tension) -> Float {
  return SPRING::LOOSE + (SPRING::TAUT - SPRING::LOOSE) * tension;
}

}  // namespace

void SOUND::SPRING::build(Effect &effect) {
  for (Whole side = 0; side < CORE::REVERB::SIDES; ++side)
    build(effect.springs[side], LONGEST * LENGTHS[side], effect.rate);
  CORE::FILTER::settle(effect.cut, CORE::FILTER::HIGH, CUT, effect.rate);
}

void SOUND::SPRING::settle(Effect &effect) {
  const Float tension = effect.rows[TENSION];
  for (Whole side = 0; side < CORE::REVERB::SIDES; ++side)
    settle(
      effect.springs[side], ::seconds(tension) * LENGTHS[side],
      ::chirp(tension), effect.rows[DECAY], effect.rate);
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
