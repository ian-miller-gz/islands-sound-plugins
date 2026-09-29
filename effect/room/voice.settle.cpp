// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::ROOM::build(Effect &effect) {
  CORE::REVERB::build(effect.reflections, effect.rate);
  CORE::REVERB::build(effect.network, CORE::REVERB::FOUR, effect.rate);
}

void SOUND::ROOM::settle(Effect &effect) {
  CORE::REVERB::settle(effect.reflections, effect.rows[SIZE]);
  CORE::REVERB::settle(
    effect.network, effect.rows[SIZE], effect.rows[DECAY], DAMP);
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
