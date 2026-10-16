// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;

void kept(OPERATOR::Synth &, Whole) {}

void struck(OPERATOR::Synth &synth, Whole at) {
  OPERATOR::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  OPERATOR::fit(synth, at);
  for (Whole unit = 0; unit < OPERATOR::OPERATORS; ++unit) {
    CORE::ENVELOPE::strike(voice.walks[unit], voice.segments[unit], velocity);
    CORE::OSCILLATOR::reset(voice.units[unit], 0);
    voice.outs[unit] = 0;
  }
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  if (synth.rows[OPERATOR::SYNC] > HALF) CORE::MODULATOR::reset(synth.lfo);
  voice.stale = true;
}

void tied(OPERATOR::Synth &synth, Whole at) { synth.voices[at].stale = true; }

void lifted(OPERATOR::Synth &synth, Whole at) {
  for (CORE::ENVELOPE::Walk &walk : synth.voices[at].walks)
    CORE::ENVELOPE::lift(walk);
  CORE::ENVELOPE::lift(synth.voices[at].contour);
}

using Visit = void (*)(OPERATOR::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, tied, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(OPERATOR::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= OPERATOR::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

void release(OPERATOR::Synth &synth) {
  const auto visitor = [&synth](const CORE::VOICE::Note &note) {
    ::visit(synth, note);
  };
  CORE::VOICE::hold(synth.allocator, false, visitor);
  for (Whole pitch = 0; pitch < CORE::PHASE::PITCHES; ++pitch)
    CORE::VOICE::lift(synth.allocator, pitch);
  CORE::VOICE::drain(synth.allocator, visitor);
}

void sustain(OPERATOR::Synth &synth) {
  CORE::VOICE::hold(
    synth.allocator, synth.rows[OPERATOR::HOLD] > HALF,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
}

}  // namespace

void SOUND::PLUGINS::OPERATOR::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  const Flag moved = mode(synth) != synth.allocator.mode;
  if (moved) ::release(synth);
  settle(synth);
  if (moved || row == HOLD) ::sustain(synth);
}
