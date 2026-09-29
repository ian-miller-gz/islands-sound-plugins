// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole CHANGES = CORE::VOICE::LIFTED + 1;

void kept(AMPLIFIER::Module &, const CORE::VOICE::Note &) {}

void struck(AMPLIFIER::Module &module, const CORE::VOICE::Note &note) {
  CORE::ENVELOPE::strike(module.gate, module.envelope, note.velocity);
}

void tied(AMPLIFIER::Module &module, const CORE::VOICE::Note &note) {
  CORE::ENVELOPE::tie(module.gate, module.envelope, note.velocity);
}

void lifted(AMPLIFIER::Module &module, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(module.gate);
}

using Change = void (*)(AMPLIFIER::Module &, const CORE::VOICE::Note &);

constexpr Change CHANGED[CHANGES] = {kept, struck, tied, lifted};

}  // namespace

void SOUND::AMPLIFIER::seat(Module &module) {
  CORE::VOICE::Allocator &allocator = module.allocator;
  allocator.count = 1;
  allocator.mode = CORE::VOICE::MONO;
  allocator.priority = CORE::VOICE::LAST;
  allocator.legato = true;
  allocator.unison = 1;
  module.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  module.envelope.trigger = CORE::ENVELOPE::RESUME;
}

void SOUND::AMPLIFIER::settle(Module &module) {
  CORE::ENVELOPE::Envelope &envelope = module.envelope;
  envelope.attack = module.rows[ATTACK];
  envelope.decay = module.rows[DECAY];
  envelope.sustain = module.rows[SUSTAIN];
  envelope.release = module.rows[RELEASE];
  envelope.depth = module.rows[VELOCITY];
  CORE::ENVELOPE::shape(envelope, module.rate);
}

void SOUND::AMPLIFIER::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  const auto visit = [&module](const CORE::VOICE::Note &note) {
    if (note.change < CHANGES) CHANGED[note.change](module, note);
  };
  const Whole row = CORE::VOICE::apply(module.allocator, event, visit);
  if (row >= PARAMETERS) return;
  module.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(module);
}
