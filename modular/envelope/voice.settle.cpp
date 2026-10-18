// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole CHANGES = CORE::VOICE::LIFTED + 1;

void kept(ENVELOPE::Module &, const CORE::VOICE::Note &) {}

void struck(ENVELOPE::Module &module, const CORE::VOICE::Note &note) {
  ENVELOPE::strike(module, note.velocity);
}

void tied(ENVELOPE::Module &module, const CORE::VOICE::Note &note) {
  if (module.left.delay > 0) return;
  CORE::ENVELOPE::tie(module.gate, module.envelope, note.velocity);
}

void lifted(ENVELOPE::Module &module, const CORE::VOICE::Note &) {
  if (!module.edge.high) ENVELOPE::lift(module);
}

using Change = void (*)(ENVELOPE::Module &, const CORE::VOICE::Note &);

constexpr Change CHANGED[CHANGES] = {kept, struck, tied, lifted};

constexpr Whole TIMES[] = {
  ENVELOPE::ATTACK, ENVELOPE::DECAY, ENVELOPE::RELEASE, ENVELOPE::DELAY,
  ENVELOPE::HOLD};

void stretch(const ENVELOPE::Module &module, Float *timed) {
  const Float *rows = module.rows;
  const Float octaves = -rows[ENVELOPE::TIME];
  for (Whole row = 0; row < ENVELOPE::PARAMETERS; ++row) timed[row] = rows[row];
  for (const Whole row : TIMES) {
    const CORE::TABLE::Row &described = ENVELOPE::ROWS[row];
    timed[row] = CORE::BLOCK::scaled(
      rows[row], module.cv, octaves, described.least, described.most);
  }
}

}  // namespace

void SOUND::PLUGINS::ENVELOPE::seat(Module &module) {
  CORE::VOICE::Allocator &allocator = module.allocator;
  allocator.count = 1;
  allocator.mode = CORE::VOICE::MONO;
  allocator.priority = CORE::VOICE::LAST;
  allocator.legato = true;
  allocator.unison = 1;
}

void SOUND::PLUGINS::ENVELOPE::settle(Module &module) {
  Float timed[PARAMETERS] = {};
  ::stretch(module, timed);
  module.envelope = shaped(timed);
  CORE::ENVELOPE::shape(module.envelope, module.rate);
  module.spans = spanned(timed, module.rate);
}

void SOUND::PLUGINS::ENVELOPE::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  const auto visit = [&module](const CORE::VOICE::Note &note) {
    if (note.change < CHANGES) CHANGED[note.change](module, note);
  };
  const Whole row = CORE::VOICE::apply(module.allocator, event, visit);
  if (row >= PARAMETERS) return;
  module.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(module);
}
