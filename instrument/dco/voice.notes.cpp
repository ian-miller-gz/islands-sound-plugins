// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto alone(const DCO::Synth &synth, Whole at) -> Flag {
  const CORE::VOICE::Note *notes = synth.allocator.notes;
  for (Whole other = 0; other < DCO::VOICES; ++other)
    if (notes[other].held && notes[other].struck != notes[at].struck)
      return false;
  return true;
}

void kept(DCO::Synth &, Whole) {}

void struck(DCO::Synth &synth, Whole at) {
  DCO::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.filter.gate, voice.filter.shape, velocity);
  CORE::ENVELOPE::strike(voice.amplifier.gate, voice.amplifier.shape, velocity);
  CORE::BLOCK::reset(voice.stride);
  if (::alone(synth, at)) CORE::MODULATOR::reset(synth.lfo);
}

void tied(DCO::Synth &synth, Whole at) {
  DCO::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::tie(voice.filter.gate, voice.filter.shape, velocity);
  CORE::ENVELOPE::tie(voice.amplifier.gate, voice.amplifier.shape, velocity);
}

void lifted(DCO::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].filter.gate);
  CORE::ENVELOPE::lift(synth.voices[at].amplifier.gate);
}

using Visit = void (*)(DCO::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, tied, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(DCO::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= DCO::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

void release(DCO::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  for (CORE::VOICE::Note &note : allocator.notes) {
    if (note.held) note.change = CORE::VOICE::LIFTED;
    note.held = false;
  }
  for (Whole &key : allocator.keys) key = 0;
  CORE::VOICE::drain(allocator, [&synth](const CORE::VOICE::Note &note) {
    ::visit(synth, note);
  });
}

}  // namespace

void SOUND::PLUGINS::DCO::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  const Float before = synth.rows[row];
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
  if (row == ASSIGN && synth.rows[row] != before) ::release(synth);
}
