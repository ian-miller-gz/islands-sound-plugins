// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void kept(MONO::Synth &, const CORE::VOICE::Note &) {}

void struck(MONO::Synth &synth, const CORE::VOICE::Note &note) {
  MONO::strike(synth.envelope, note.velocity);
  MONO::strike(synth.gate, note.velocity);
}

void lifted(MONO::Synth &synth, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(synth.envelope.gate);
  CORE::ENVELOPE::lift(synth.gate.gate);
}

using Visit = void (*)(MONO::Synth &, const CORE::VOICE::Note &);

constexpr Visit VISITS[] = {kept, struck, kept, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(MONO::Synth &synth, const CORE::VOICE::Note &note) {
  if (note.change < std::size(VISITS)) VISITS[note.change](synth, note);
}

}  // namespace

void SOUND::MONO::strike(Contour &contour, Float velocity) {
  CORE::ENVELOPE::strike(contour.gate, contour.envelope, velocity);
}

void SOUND::MONO::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
