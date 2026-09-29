// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto alone(const DCO::Synth &synth, Whole at) -> Flag {
  for (Whole other = 0; other < DCO::VOICES; ++other)
    if (other != at && synth.allocator.notes[other].held) return false;
  return true;
}

void kept(DCO::Synth &, Whole) {}

void struck(DCO::Synth &synth, Whole at) {
  DCO::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  CORE::ENVELOPE::strike(voice.door, synth.door, velocity);
  if (::alone(synth, at)) CORE::MODULATOR::reset(synth.lfo);
}

void lifted(DCO::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour);
  CORE::ENVELOPE::lift(synth.voices[at].door);
}

using Visit = void (*)(DCO::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(DCO::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= DCO::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::PLUGINS::DCO::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
