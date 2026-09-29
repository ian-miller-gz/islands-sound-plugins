// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds * HALL::MILLISECOND * Float(rate);
}

auto cutoff(Float damp) -> Float {
  if (damp <= 0) return 0;
  return HALL::BRIGHTEST * std::pow(HALL::DARKEST / HALL::BRIGHTEST, damp);
}

}  // namespace

void SOUND::HALL::build(Effect &effect) {
  const Float longest = CORE::TABLE::found(SHEET, PREDELAY)->most;
  CORE::REVERB::build(
    effect.predelay, Whole(std::ceil(::frames(longest, effect.rate))) + 1);
  CORE::REVERB::build(effect.network, CORE::REVERB::EIGHT, effect.rate);
}

void SOUND::HALL::settle(Effect &effect) {
  CORE::REVERB::settle(
    effect.predelay, ::frames(effect.rows[PREDELAY], effect.rate));
  CORE::REVERB::settle(
    effect.network, effect.rows[SIZE], effect.rows[DECAY],
    ::cutoff(effect.rows[DAMP]));
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
