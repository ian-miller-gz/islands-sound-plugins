// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds * PLATE::MILLISECOND * Float(rate);
}

}  // namespace

void SOUND::PLATE::build(Effect &effect) {
  const Float longest = CORE::TABLE::found(SHEET, PREDELAY)->most;
  CORE::REVERB::build(
    effect.predelay, Whole(std::ceil(::frames(longest, effect.rate))) + 1);
  CORE::REVERB::build(effect.plate, effect.rate);
}

void SOUND::PLATE::settle(Effect &effect) {
  CORE::REVERB::settle(
    effect.predelay, ::frames(effect.rows[PREDELAY], effect.rate));
  CORE::REVERB::settle(
    effect.plate, effect.rows[DECAY], effect.rows[DAMP], BANDWIDTH,
    effect.rows[DIFFUSION]);
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
