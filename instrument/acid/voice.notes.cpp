// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float ACCENTED = 100.0f / SOUND::PLUGIN::FULL;

void kept(ACID::Synth &, const CORE::VOICE::Note &) {}

void struck(ACID::Synth &synth, const CORE::VOICE::Note &note) {
  synth.accent = note.velocity >= ACCENTED ? ACID::STRONG : ACID::PLAIN;
  CORE::ENVELOPE::strike(
    synth.contour, synth.contours[synth.accent], note.velocity);
  CORE::ENVELOPE::strike(synth.door, synth.amplifier, note.velocity);
}

void lifted(ACID::Synth &synth, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(synth.contour);
  CORE::ENVELOPE::lift(synth.door);
}

using Visit = void (*)(ACID::Synth &, const CORE::VOICE::Note &);

constexpr Visit VISITS[] = {kept, struck, kept, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(ACID::Synth &synth, const CORE::VOICE::Note &note) {
  if (note.change < std::size(VISITS)) VISITS[note.change](synth, note);
}

}  // namespace

void SOUND::PLUGINS::ACID::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
