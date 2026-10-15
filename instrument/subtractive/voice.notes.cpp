// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(SUBTRACTIVE::Synth &, Whole) {}

void struck(SUBTRACTIVE::Synth &synth, Whole at) {
  SUBTRACTIVE::Voice &voice = synth.voices[at];
  for (uint32_t &phase : voice.phases) phase = 0;
  voice.low = 0;
  voice.band = 0;
  for (SUBTRACTIVE::Gate &gate : voice.gates) gate = {0, SUBTRACTIVE::RISING};
}

void tied(SUBTRACTIVE::Synth &synth, Whole at) {
  for (SUBTRACTIVE::Gate &gate : synth.voices[at].gates)
    if (gate.stage == SUBTRACTIVE::IDLE) gate = {0, SUBTRACTIVE::RISING};
}

void lifted(SUBTRACTIVE::Synth &synth, Whole at) {
  for (SUBTRACTIVE::Gate &gate : synth.voices[at].gates)
    if (gate.stage != SUBTRACTIVE::IDLE) gate.stage = SUBTRACTIVE::LEAVING;
}

using Visit = void (*)(SUBTRACTIVE::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, tied, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(SUBTRACTIVE::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= SUBTRACTIVE::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

void release(SUBTRACTIVE::Synth &synth) {
  for (Whole pitch = 0; pitch < CORE::PHASE::PITCHES; ++pitch)
    CORE::VOICE::lift(synth.allocator, pitch);
  CORE::VOICE::drain(synth.allocator, [&synth](const CORE::VOICE::Note &note) {
    ::visit(synth, note);
  });
}

}  // namespace

void SOUND::PLUGINS::SUBTRACTIVE::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  if (mode(synth) != synth.allocator.mode) ::release(synth);
  settle(synth);
}
