// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using AUDIO::PLUGIN::Event;

void kept(POLYMOD::Synth &, Whole) {}

void struck(POLYMOD::Synth &synth, Whole at) {
  POLYMOD::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour.gate, voice.contour.shape, velocity);
  CORE::ENVELOPE::strike(voice.loudness.gate, voice.loudness.shape, velocity);
}

void lifted(POLYMOD::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour.gate);
  CORE::ENVELOPE::lift(synth.voices[at].loudness.gate);
}

using Visit = void (*)(POLYMOD::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(POLYMOD::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= POLYMOD::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

auto route(POLYMOD::Synth &synth, const Event &event) -> Whole {
  return CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
}

void sustain(POLYMOD::Synth &synth) {
  CORE::VOICE::hold(
    synth.allocator, synth.rows[POLYMOD::HOLD] > 0,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
}

}  // namespace

void SOUND::PLUGINS::POLYMOD::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = ::route(synth, event);
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
  if (row == HOLD) ::sustain(synth);
}
