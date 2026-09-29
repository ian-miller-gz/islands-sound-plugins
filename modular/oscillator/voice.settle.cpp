// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::OSCILLATOR::seat(Module &module) {
  CORE::VOICE::Allocator &allocator = module.allocator;
  allocator.count = 1;
  allocator.mode = CORE::VOICE::MONO;
  allocator.priority = CORE::VOICE::LAST;
  allocator.legato = true;
  allocator.unison = 1;
  allocator.glide.slide = CORE::VOICE::ALWAYS;
  CORE::OSCILLATOR::scatter(module.super, 0);
}

void SOUND::OSCILLATOR::settle(Module &module) {
  module.allocator.glide.time = module.rows[GLIDE];
  CORE::VOICE::settle(module.allocator, module.rate);
  module.stale = true;
}

void SOUND::OSCILLATOR::tune(Module &module, Float pitch) {
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float width = module.rows[WIDTH];
  CORE::OSCILLATOR::settle(module.oscillator, hertz, width, module.rate);
  CORE::OSCILLATOR::settle(module.super, hertz, width, MIX, module.rate);
  module.pitch = pitch;
  module.stale = false;
}

void SOUND::OSCILLATOR::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  const auto struck = [&module](const CORE::VOICE::Note &note) {
    if (note.change == CORE::VOICE::STRUCK) module.sounding = true;
  };
  const Whole row = CORE::VOICE::apply(module.allocator, event, struck);
  if (row >= PARAMETERS) return;
  module.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(module);
}
