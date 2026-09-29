// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;

}  // namespace

void SOUND::PLUGINS::CHOIR::place(Choir &choir) {
  const Float *rows = choir.rows;
  choir.place = rows[VOWEL] + rows[MORPH] * (FULL + choir.sway) * HALF;
}

void SOUND::PLUGINS::CHOIR::tune(
  const Choir &choir, const CORE::VOICE::Note &note, Part &part) {
  const Float detune = choir.rows[DETUNE];
  for (Singer &singer : part.singers) {
    const Float vibrato = CORE::MODULATOR::tick(singer.vibrato);
    const Float cents = singer.seat.detune * detune + vibrato;
    const Float pitch = Float(note.pitch) + cents / CENT;
    CORE::GLOTTIS::tune(singer.source, CORE::PHASE::hertz(pitch), choir.rate);
    CORE::FILTER::settle(singer.formant, choir.place, singer.shift, choir.rate);
  }
}

void SOUND::PLUGINS::CHOIR::steer(Choir &choir) {
  choir.sway = CORE::MODULATOR::tick(choir.motion);
  place(choir);
  for (Whole at = 0; at < NOTES; ++at) {
    const CORE::VOICE::Note &note = choir.allocator.notes[at];
    if (note.sounding) tune(choir, note, choir.parts[at]);
  }
}
