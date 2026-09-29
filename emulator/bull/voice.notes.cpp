// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Integer BOTTOM = 36;
constexpr Integer TOP = 48;
constexpr Integer OCTAVE = 12;

auto folded(Whole pitch) -> Whole {
  Integer key = Integer(pitch);
  if (key < BOTTOM) key += OCTAVE * ((BOTTOM - key + OCTAVE - 1) / OCTAVE);
  if (key > TOP) key -= OCTAVE * ((key - TOP + OCTAVE - 1) / OCTAVE);
  return Whole(key);
}

auto keyed(const AUDIO::PLUGIN::Event &event) -> Flag {
  return event.kind == AUDIO::PLUGIN::Event::NOTE_ON ||
         event.kind == AUDIO::PLUGIN::Event::NOTE_OFF;
}

auto ranged(const BULL::Synth &synth, const AUDIO::PLUGIN::Event &event)
  -> AUDIO::PLUGIN::Event {
  AUDIO::PLUGIN::Event moved = event;
  const Flag low = Whole(synth.rows[BULL::RANGE]) == BULL::LOW;
  if (low && keyed(event) && event.index < CORE::PHASE::PITCHES)
    moved.index = folded(event.index);
  return moved;
}

void kept(BULL::Synth &, const CORE::VOICE::Note &) {}

void struck(BULL::Synth &synth, const CORE::VOICE::Note &note) {
  for (BULL::Contour *contour : {&synth.loudness, &synth.filter})
    CORE::ENVELOPE::strike(contour->gate, contour->envelope, note.velocity);
}

void lifted(BULL::Synth &synth, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(synth.loudness.gate);
  CORE::ENVELOPE::lift(synth.filter.gate);
}

using Visit = void (*)(BULL::Synth &, const CORE::VOICE::Note &);

constexpr Visit VISITS[] = {kept, struck, kept, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(BULL::Synth &synth, const CORE::VOICE::Note &note) {
  if (note.change < std::size(VISITS)) VISITS[note.change](synth, note);
}

}  // namespace

void SOUND::PLUGINS::BULL::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, ::ranged(synth, event),
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
