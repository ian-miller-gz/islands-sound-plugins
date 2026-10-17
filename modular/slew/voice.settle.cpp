// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SLEW::settle(Module &module) {
  for (Whole channel = 0; channel < module.channels; ++channel)
    tune(module, channel, 0);
}

void SOUND::PLUGINS::SLEW::tune(Module &module, Whole channel, Float cv) {
  CORE::MODULATOR::Slew &slew = module.slews[channel];
  const Float *rows = module.rows;
  const Float octaves = -rows[AMOUNT];
  slew.rise = CORE::BLOCK::scaled(rows[RISE], cv, octaves, QUICKEST, SLOWEST);
  slew.fall = CORE::BLOCK::scaled(rows[FALL], cv, octaves, QUICKEST, SLOWEST);
  slew.shape = rows[SHAPE];
  CORE::MODULATOR::settle(slew, module.rate);
}

void SOUND::PLUGINS::SLEW::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
}
