// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chord.hpp"
#include "../../core/notes/notes.hpp"

namespace SOUND::CHORD {

struct Chord {
  Float rows[PARAMETERS] = {};
  Whole voicings[CORE::NOTES::KEYS][VOICES] = {};
  CORE::NOTES::Tally tally;
};

void voice(const Chord &chord, Whole key, Whole *pitches);
void apply(
  Chord &chord, const AUDIO::PLUGIN::Event &event, CORE::NOTES::Out &out);

auto answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

}  // namespace SOUND::CHORD
