// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(SUBTRACTIVE::Synth &, Whole) {}

void struck(SUBTRACTIVE::Synth &synth, Whole at) {
  SUBTRACTIVE::Voice &voice = synth.voices[at];
  for (CORE::OSCILLATOR::Oscillator &oscillator : voice.oscillators)
    CORE::OSCILLATOR::reset(oscillator, 0);
  voice.variable = {};
  const Float velocity = synth.allocator.notes[at].velocity;
  for (Whole curve = 0; curve < SUBTRACTIVE::CURVES; ++curve)
    CORE::ENVELOPE::strike(
      voice.gates[curve], synth.envelopes[curve], velocity);
}

void tied(SUBTRACTIVE::Synth &synth, Whole at) {
  SUBTRACTIVE::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  for (Whole curve = 0; curve < SUBTRACTIVE::CURVES; ++curve)
    CORE::ENVELOPE::tie(voice.gates[curve], synth.envelopes[curve], velocity);
}

void lifted(SUBTRACTIVE::Synth &synth, Whole at) {
  for (CORE::ENVELOPE::Gate &gate : synth.voices[at].gates)
    CORE::ENVELOPE::lift(gate);
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
