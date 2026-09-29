// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::CHORUS::settle(Effect &effect) {
  effect.voices = FEWEST + Whole(effect.rows[VOICES]);
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX] / Float(effect.voices);
  const Float depth = effect.rows[DEPTH] * MILLISECOND;
  for (Channel &strip : effect.strips)
    for (Whole voice = 0; voice < MOST; ++voice) {
      strip.lfos[voice].wave = CORE::MODULATOR::TRIANGLE;
      strip.lfos[voice].hertz = effect.rows[RATE];
      CORE::MODULATOR::settle(strip.lfos[voice], effect.rate);
      CORE::LINE::settle(
        strip.sweeps[voice], CENTRE, depth, GLIDE, effect.rate);
    }
}

void SOUND::PLUGINS::CHORUS::place(Effect &effect) {
  const Float gap = UNITY / Float(effect.voices);
  const Float offset = effect.rows[SPREAD] * gap / Float(effect.channels);
  for (Whole channel = 0; channel < effect.channels; ++channel)
    for (Whole voice = 0; voice < effect.voices; ++voice) {
      CORE::MODULATOR::Lfo &lfo = effect.strips[channel].lfos[voice];
      lfo.start = gap * Float(voice) + offset * Float(channel);
      CORE::MODULATOR::reset(lfo);
    }
}
