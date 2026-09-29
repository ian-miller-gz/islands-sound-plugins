// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SLEW::settle(Module &module) {
  for (CORE::MODULATOR::Slew &slew : module.slews) {
    slew.rise = module.rows[RISE];
    slew.fall = module.rows[FALL];
    CORE::MODULATOR::settle(slew, module.rate);
  }
}

void SOUND::PLUGINS::SLEW::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
