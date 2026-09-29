// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::CRUSHER::build(Effect &effect) {
  effect.crushers.resize(effect.channels);
}

void SOUND::PLUGINS::CRUSHER::settle(Effect &effect) {
  for (auto &crusher : effect.crushers)
    CORE::SHAPER::settle(
      crusher, effect.rows[BITS], effect.rows[RATE], effect.rate);
  effect.stride = effect.crushers.front().stride;
  effect.sway = SWAY * effect.rows[JITTER];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
