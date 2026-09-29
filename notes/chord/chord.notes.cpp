// SPDX-License-Identifier: AGPL-3.0-or-later
#include "chord.internal.hpp"

namespace {
using namespace SOUND;

void lift(CHORD::Chord &chord, Whole key, Whole offset, CORE::NOTES::Out &out) {
  for (Whole &pitch : chord.voicings[key]) {
    if (pitch == CORE::NOTES::SILENT) continue;
    CORE::NOTES::lift(chord.tally, out, pitch, offset);
    pitch = CORE::NOTES::SILENT;
  }
}

void strike(
  CHORD::Chord &chord, const AUDIO::PLUGIN::Event &event,
  CORE::NOTES::Out &out) {
  ::lift(chord, event.index, event.offset, out);
  Whole *pitches = chord.voicings[event.index];
  CHORD::voice(chord, event.index, pitches);
  for (Whole voice = 0; voice < CHORD::VOICES; ++voice)
    CORE::NOTES::strike(
      chord.tally, out, pitches[voice], event.offset, event.value);
}

void turn(CHORD::Chord &chord, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= CHORD::PARAMETERS) return;
  chord.rows[event.index] =
    CORE::TABLE::clamped(CHORD::SHEET, event.index, event.value);
}

}  // namespace

void SOUND::CHORD::apply(
  Chord &chord, const AUDIO::PLUGIN::Event &event, CORE::NOTES::Out &out) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER)
    return ::turn(chord, event);
  const Flag struck = CORE::NOTES::struck(event);
  if (!struck && !CORE::NOTES::lifted(event)) {
    CORE::NOTES::put(out, event);
    return;
  }
  if (event.index > CORE::NOTES::HIGHEST) return;
  if (struck) return ::strike(chord, event, out);
  ::lift(chord, event.index, event.offset, out);
}
