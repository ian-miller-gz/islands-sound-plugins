// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

void SOUND::WIDENER::build(Effect &effect) {
  effect.strips.resize(effect.channels / SIDES);
}

void SOUND::WIDENER::settle(Effect &effect) {
  effect.mid = CORE::DYNAMICS::gain(effect.rows[MID]);
  effect.side = effect.rows[WIDTH] * CORE::DYNAMICS::gain(effect.rows[SIDE]);
  for (Strip &strip : effect.strips)
    for (CORE::FILTER::Biquad &high : strip.highs)
      CORE::FILTER::settle(
        high, CORE::FILTER::HIGH, effect.rows[BASS], CORE::FILTER::FLAT, 0,
        effect.rate);
}
