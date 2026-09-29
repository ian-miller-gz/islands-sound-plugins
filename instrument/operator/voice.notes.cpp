// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float HALF = 0.5f;

void kept(OPERATOR::Synth &, Whole) {}

void struck(OPERATOR::Synth &synth, Whole at) {
  OPERATOR::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  for (Whole unit = 0; unit < OPERATOR::OPERATORS; ++unit) {
    CORE::ENVELOPE::strike(voice.walks[unit], synth.segments[unit], velocity);
    CORE::OSCILLATOR::reset(voice.units[unit], 0);
    voice.outs[unit] = 0;
  }
  if (synth.rows[OPERATOR::SYNC] > HALF) CORE::MODULATOR::reset(synth.lfo);
  voice.stale = true;
}

void lifted(OPERATOR::Synth &synth, Whole at) {
  for (CORE::ENVELOPE::Walk &walk : synth.voices[at].walks)
    CORE::ENVELOPE::lift(walk);
}

using Visit = void (*)(OPERATOR::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(OPERATOR::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= OPERATOR::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::OPERATOR::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
