// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(CHIP::Synth &) {}

void tied(CHIP::Synth &synth) { synth.stale = true; }

using Visit = void (*)(CHIP::Synth &);

constexpr Visit VISITS[] = {kept, CHIP::strike, tied, CHIP::lift};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(CHIP::Synth &synth, const CORE::VOICE::Note &note) {
  if (note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth);
}

}  // namespace

void SOUND::PLUGINS::CHIP::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
