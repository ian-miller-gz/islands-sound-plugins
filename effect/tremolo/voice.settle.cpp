// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

void SOUND::TREMOLO::settle(Effect &effect) {
  const CORE::MODULATOR::Wave wave = WAVES[Whole(effect.rows[SHAPE])];
  for (Channel &strip : effect.strips) {
    strip.lfo.wave = wave;
    strip.lfo.hertz = effect.rows[RATE];
    CORE::MODULATOR::settle(strip.lfo, effect.rate);
  }
}

void SOUND::TREMOLO::place(Effect &effect) {
  const Float anchor = CORE::PHASE::fraction(effect.strips.front().lfo.phase);
  const Float offset = effect.rows[PHASE] / DEGREES;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    const Float start = anchor + offset * Float(channel % SIDES);
    CORE::MODULATOR::Lfo &lfo = effect.strips[channel].lfo;
    lfo.start = start - std::floor(start);
    CORE::MODULATOR::reset(lfo);
  }
}
