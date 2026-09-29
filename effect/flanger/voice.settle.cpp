// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::FLANGER::settle(Effect &effect) {
  effect.sign = effect.rows[POLARITY] > 0 ? -UNITY : UNITY;
  effect.lfo.hertz = effect.rows[RATE];
  CORE::MODULATOR::settle(effect.lfo, effect.rate);
  const Float swing = effect.rows[DEPTH] * HALF * MILLISECOND;
  const Float centre = effect.rows[MANUAL] * MILLISECOND + swing;
  const Float feedback = effect.sign * effect.rows[FEEDBACK];
  for (Channel &strip : effect.strips) {
    CORE::LINE::settle(strip.sweep, centre, swing, GLIDE, effect.rate);
    CORE::LINE::settle(strip.loop, feedback, OPEN, effect.rate);
  }
}
