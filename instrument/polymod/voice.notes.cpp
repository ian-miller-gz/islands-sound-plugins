// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using AUDIO::PLUGIN::Event;

void kept(POLYMOD::Synth &, Whole) {}

void struck(POLYMOD::Synth &synth, Whole at) {
  POLYMOD::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(voice.contour, synth.contour, velocity);
  CORE::ENVELOPE::strike(voice.loudness, synth.loudness, velocity);
}

void lifted(POLYMOD::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].contour);
  CORE::ENVELOPE::lift(synth.voices[at].loudness);
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

auto lifting(const Event &event) -> Flag {
  if (event.kind == Event::NOTE_OFF) return true;
  return event.kind == Event::NOTE_ON && event.value <= 0;
}

auto deferred(POLYMOD::Synth &synth, const Event &event) -> Flag {
  if (event.index >= CORE::PHASE::PITCHES) return false;
  const Flag lift = ::lifting(event);
  if (event.kind == Event::NOTE_ON && !lift) synth.lifts[event.index] = false;
  if (!lift || synth.rows[POLYMOD::HOLD] <= 0) return false;
  synth.lifts[event.index] = true;
  return true;
}

void release(POLYMOD::Synth &synth) {
  for (Whole pitch = 0; pitch < CORE::PHASE::PITCHES; ++pitch) {
    if (!synth.lifts[pitch]) continue;
    synth.lifts[pitch] = false;
    ::route(synth, {.kind = Event::NOTE_OFF, .index = pitch});
  }
}

}  // namespace

void SOUND::PLUGINS::POLYMOD::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (::deferred(synth, event)) return;
  const Whole row = ::route(synth, event);
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
  if (row == HOLD && synth.rows[HOLD] <= 0) ::release(synth);
}
