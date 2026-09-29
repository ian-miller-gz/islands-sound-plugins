// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(MULTIMODE::Synth &, Whole) {}

void struck(MULTIMODE::Synth &synth, Whole at) {
  MULTIMODE::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  CORE::ENVELOPE::strike(voice.loudness, synth.loudness, velocity);
}

void lifted(MULTIMODE::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour);
  CORE::ENVELOPE::lift(synth.voices[at].loudness);
}

using Visit = void (*)(MULTIMODE::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(MULTIMODE::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= MULTIMODE::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::PLUGINS::MULTIMODE::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
