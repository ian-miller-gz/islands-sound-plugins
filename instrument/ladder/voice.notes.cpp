// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(LADDER::Synth &, const CORE::VOICE::Note &) {}

void struck(LADDER::Synth &synth, const CORE::VOICE::Note &note) {
  for (LADDER::Contour *contour : {&synth.loudness, &synth.filter})
    CORE::ENVELOPE::strike(contour->gate, contour->envelope, note.velocity);
}

void lifted(LADDER::Synth &synth, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(synth.loudness.gate);
  CORE::ENVELOPE::lift(synth.filter.gate);
}

using Visit = void (*)(LADDER::Synth &, const CORE::VOICE::Note &);

constexpr Visit VISITS[] = {kept, struck, kept, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(LADDER::Synth &synth, const CORE::VOICE::Note &note) {
  if (note.change < std::size(VISITS)) VISITS[note.change](synth, note);
}

}  // namespace

void SOUND::PLUGINS::LADDER::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
