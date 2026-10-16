// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto claim(ADDITIVE::Synth &synth) -> ADDITIVE::Note & {
  ADDITIVE::Note *oldest = &synth.notes[0];
  for (ADDITIVE::Note &note : synth.notes) {
    if (note.stage == ADDITIVE::IDLE) return note;
    if (note.struck < oldest->struck) oldest = &note;
  }
  return *oldest;
}

void strike(ADDITIVE::Synth &synth, const AUDIO::PLUGIN::Event &event) {
  ADDITIVE::Note &note = claim(synth);
  note.pitch = event.index < synth.steps.size() ? event.index : 0;
  note.step = synth.steps[note.pitch];
  note.velocity = event.value < 0 ? 0 : event.value > 1 ? 1 : event.value;
  note.phase = 0;
  note.level = 0;
  note.stage = ADDITIVE::RISING;
  note.struck = ++synth.clock;
}

void lift(ADDITIVE::Synth &synth, Whole pitch) {
  for (ADDITIVE::Note &note : synth.notes)
    if (note.pitch == pitch && note.stage != ADDITIVE::IDLE)
      note.stage = ADDITIVE::LEAVING;
}

void steer(ADDITIVE::Synth &synth, Whole id, Float value) {
  if (id >= ADDITIVE::PARAMETERS) return;
  synth.rows[id] = CORE::TABLE::clamped(ADDITIVE::SHEET, id, value);
  ADDITIVE::settle(synth);
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  switch (event.kind) {
    case AUDIO::PLUGIN::Event::NOTE_ON:
      if (event.value > 0)
        ::strike(synth, event);
      else
        ::lift(synth, event.index);
      break;
    case AUDIO::PLUGIN::Event::NOTE_OFF:
      ::lift(synth, event.index);
      break;
    case AUDIO::PLUGIN::Event::CONTROLLER:
      ::steer(synth, event.index, event.value);
      break;
    default:
      break;
  }
}
