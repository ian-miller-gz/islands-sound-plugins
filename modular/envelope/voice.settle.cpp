// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole CHANGES = CORE::VOICE::LIFTED + 1;

void kept(ENVELOPE::Module &, const CORE::VOICE::Note &) {}

void struck(ENVELOPE::Module &module, const CORE::VOICE::Note &note) {
  CORE::ENVELOPE::strike(module.gate, module.envelope, note.velocity);
}

void tied(ENVELOPE::Module &module, const CORE::VOICE::Note &note) {
  CORE::ENVELOPE::tie(module.gate, module.envelope, note.velocity);
}

void lifted(ENVELOPE::Module &module, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(module.gate);
}

using Change = void (*)(ENVELOPE::Module &, const CORE::VOICE::Note &);

constexpr Change CHANGED[CHANGES] = {kept, struck, tied, lifted};

auto adsr(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::ADSR::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::DECAY], rows[ENVELOPE::SUSTAIN],
    rows[ENVELOPE::RELEASE]);
}

auto ad(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::AD::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::DECAY]);
}

auto ar(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::AR::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::RELEASE]);
}

auto gate(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::GATE::create(rows[ENVELOPE::VELOCITY]);
}

using Builder = auto (*)(const Float *) -> CORE::ENVELOPE::Envelope;

constexpr Builder BUILDERS[] = {adsr, ad, ar, gate};
static_assert(std::size(BUILDERS) == std::size(ENVELOPE::SHAPES));

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
  const Float *rows = module.rows;
  const Whole shape = Whole(rows[SHAPE]);
  CORE::ENVELOPE::Envelope &envelope = module.envelope;
  envelope = ::BUILDERS[shape <= GATE ? shape : ADSR](rows);
  envelope.curve = Whole(rows[CURVE]);
  envelope.trigger = Whole(rows[TRIGGER]);
  envelope.depth = rows[VELOCITY];
  CORE::ENVELOPE::shape(envelope, module.rate);
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
