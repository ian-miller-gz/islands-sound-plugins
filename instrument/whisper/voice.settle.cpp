// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::WHISPER::seed(Hiss &hiss, CORE::NOISE::Register state) {
  CORE::NOISE::seed(hiss.white, state);
  CORE::NOISE::seed(hiss.pink, state);
  CORE::NOISE::seed(hiss.brown, state);
  CORE::NOISE::seed(hiss.blue, state);
}

void SOUND::WHISPER::shape(Mouth &mouth, Whole at) {
  if (at >= VOICES) return;
  const Float *rows = mouth.rows;
  const Float key = Float(mouth.allocator.notes[at].pitch) - CENTRE;
  const Float shift = CORE::PHASE::ratio(rows[TRACKING] * key * CENT);
  CORE::FILTER::settle(
    mouth.hisses[at].formant, rows[VOWEL] + rows[MORPH], shift, mouth.rate);
}

void SOUND::WHISPER::settle(Mouth &mouth) {
  const Float *rows = mouth.rows;
  mouth.gain = rows[GAIN];
  mouth.colour = Whole(rows[COLOUR]);
  mouth.envelope = CORE::ENVELOPE::ADSR::create(
    rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]);
  mouth.envelope.depth = CORE::ENVELOPE::FULL;
  CORE::ENVELOPE::shape(mouth.envelope, mouth.rate);
  for (Whole at = 0; at < VOICES; ++at) shape(mouth, at);
}
