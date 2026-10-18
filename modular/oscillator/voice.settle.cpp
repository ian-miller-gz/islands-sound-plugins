// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto noted(const OSCILLATOR::Module &module) -> Flag { return module.struck; }

auto droned(const OSCILLATOR::Module &) -> Flag { return true; }

using Rule = auto (*)(const OSCILLATOR::Module &) -> Flag;

constexpr Rule PLAYED[] = {noted, droned};
static_assert(std::size(PLAYED) == std::size(OSCILLATOR::PLAYS));

}  // namespace

void SOUND::PLUGINS::OSCILLATOR::seat(Module &module) {
  CORE::VOICE::Allocator &allocator = module.allocator;
  allocator.count = 1;
  allocator.mode = CORE::VOICE::MONO;
  allocator.priority = CORE::VOICE::LAST;
  allocator.legato = true;
  allocator.unison = 1;
  allocator.glide.slide = CORE::VOICE::ALWAYS;
  CORE::VOICE::Note &lead = allocator.notes[LEAD];
  lead.pitch = MIDDLE;
  lead.current = Float(MIDDLE);
  CORE::OSCILLATOR::scatter(module.super, 0);
}

void SOUND::PLUGINS::OSCILLATOR::settle(Module &module) {
  module.allocator.glide.time = module.rows[GLIDE];
  CORE::VOICE::settle(module.allocator, module.rate);
  module.stale = true;
}

void SOUND::PLUGINS::OSCILLATOR::tune(Module &module, Float pitch, Float fm) {
  const Float hertz = modulated(module, pitch, fm);
  const Float *rows = module.rows;
  CORE::OSCILLATOR::settle(module.sync.slave, hertz, rows[WIDTH], module.rate);
  CORE::OSCILLATOR::settle(module.super, hertz, rows[DETUNE], MIX, module.rate);
  module.pitch = pitch;
  module.fm = fm;
  module.stale = false;
}

void SOUND::PLUGINS::OSCILLATOR::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  const auto visit = [&module](const CORE::VOICE::Note &note) {
    if (note.change == CORE::VOICE::STRUCK) module.struck = true;
  };
  const Whole row = CORE::VOICE::apply(module.allocator, event, visit);
  if (row >= PARAMETERS) return;
  module.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(module);
}

auto SOUND::PLUGINS::OSCILLATOR::playing(const Module &module) -> Flag {
  const Whole play = Whole(module.rows[PLAY]);
  return ::PLAYED[play < std::size(::PLAYED) ? play : NOTES](module);
}
