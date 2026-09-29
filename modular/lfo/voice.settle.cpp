// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::LFO::settle(Module &module) {
  const Float *rows = module.rows;
  CORE::MODULATOR::Lfo &lfo = module.lfo;
  lfo.wave = Whole(rows[WAVE]);
  lfo.hertz = rows[RATE];
  lfo.depth = rows[DEPTH];
  lfo.start = rows[PHASE];
  lfo.fade = rows[FADE];
  lfo.slew = CORE::MODULATOR::FULL / rows[RATE];
  CORE::MODULATOR::settle(lfo, module.rate);
}

void SOUND::PLUGINS::LFO::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
  if (event.index == PHASE || event.index == FADE)
    CORE::MODULATOR::reset(module.lfo);
}
