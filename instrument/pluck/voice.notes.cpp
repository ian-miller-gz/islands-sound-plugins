// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float PERIODS = 1;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;

void kept(PLUCK::Synth &, Whole) {}

void struck(PLUCK::Synth &synth, Whole at) {
  PLUCK::Voice &voice = synth.voices[at];
  const CORE::VOICE::Note &note = synth.allocator.notes[at];
  PLUCK::tune(synth, at);
  const Float hertz = CORE::PHASE::hertz(Float(note.pitch) + synth.tune);
  const Float colour = synth.rows[PLUCK::TONE] * (HALF + HALF * note.velocity);
  CORE::NOISE::settle(voice.burst, PERIODS / hertz, colour, synth.rate);
  CORE::NOISE::strike(voice.burst, note.velocity);
  voice.level = FULL;
}

using Visit = void (*)(PLUCK::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, kept};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(PLUCK::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= PLUCK::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::PLUCK::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
