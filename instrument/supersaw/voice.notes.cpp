// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(SUPERSAW::Synth &, Whole) {}

void struck(SUPERSAW::Synth &synth, Whole at) {
  SUPERSAW::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  CORE::ENVELOPE::strike(voice.door, synth.door, velocity);
  CORE::MODULATOR::draw(synth.seed);
  CORE::OSCILLATOR::scatter(voice.super, synth.seed);
  voice.stale = true;
}

void lifted(SUPERSAW::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour);
  CORE::ENVELOPE::lift(synth.voices[at].door);
}

using Visit = void (*)(SUPERSAW::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(SUPERSAW::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= SUPERSAW::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::PLUGINS::SUPERSAW::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
