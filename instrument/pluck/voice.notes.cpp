// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1;

void kept(PLUCK::Synth &, Whole) {}

void struck(PLUCK::Synth &synth, Whole at) {
  PLUCK::tune(synth, at);
  PLUCK::strike(synth, at);
  synth.voices[at].level = FULL;
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

void SOUND::PLUGINS::PLUCK::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
