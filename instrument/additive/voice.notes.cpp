// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(ADDITIVE::Synth &, Whole) {}

void struck(ADDITIVE::Synth &synth, Whole at) {
  ADDITIVE::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  CORE::ENVELOPE::strike(voice.door, synth.door, velocity);
  CORE::BLOCK::reset(voice.stride);
  std::ranges::fill(voice.bank.phases, CORE::PHASE::Wheel(0));
}

void lifted(ADDITIVE::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour);
  CORE::ENVELOPE::lift(synth.voices[at].door);
}

using Visit = void (*)(ADDITIVE::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(ADDITIVE::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= ADDITIVE::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
