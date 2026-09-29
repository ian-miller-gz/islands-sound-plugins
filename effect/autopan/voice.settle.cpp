// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/clock/clock.hpp"

void SOUND::AUTOPAN::build(Effect &effect) {
  effect.smoother.time = SMOOTHING;
  CORE::MODULATOR::settle(effect.smoother, effect.rate);
  CORE::MODULATOR::jump(effect.smoother, 0);
  CORE::MODULATOR::reset(effect.lfo);
}

void SOUND::AUTOPAN::settle(Effect &effect) {
  const Float *rows = effect.rows;
  const Float beats = BEATS[Whole(rows[DIVISION])];
  effect.lfo.wave = WAVES[Whole(rows[SHAPE])];
  effect.lfo.hertz = rows[RATE];
  effect.lfo.period =
    rows[SYNC] > 0 ? CORE::CLOCK::frames(rows[TEMPO], beats, effect.rate) : 0;
  CORE::MODULATOR::settle(effect.lfo, effect.rate);
}
