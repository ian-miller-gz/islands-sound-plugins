// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

struct Shape {
  Float place;
  Float shift;
};

void voice(const VOWEL::Singer &singer, VOWEL::Throat &throat, Shape shape) {
  const Float *rows = singer.rows;
  CORE::FILTER::settle(throat.formant, shape.place, shape.shift, singer.rate);
  CORE::GLOTTIS::settle(throat.source, rows[VOWEL::OPEN], rows[VOWEL::BREATH]);
  throat.vibrato.hertz = rows[VOWEL::RATE];
  throat.vibrato.depth = rows[VOWEL::DEPTH];
  throat.vibrato.fade = rows[VOWEL::DELAY];
  CORE::MODULATOR::settle(throat.vibrato, singer.rate);
}

}  // namespace

void SOUND::VOWEL::settle(Singer &singer) {
  const Float *rows = singer.rows;
  singer.gain = rows[GAIN];
  singer.envelope = CORE::ENVELOPE::ADSR::create(
    rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]);
  singer.envelope.depth = CORE::ENVELOPE::FULL;
  CORE::ENVELOPE::shape(singer.envelope, singer.rate);
  const Shape shape = {
    rows[VOWEL] + rows[MORPH], CORE::PHASE::ratio(rows[SEX] * SPAN)};
  for (Throat &throat : singer.throats) ::voice(singer, throat, shape);
}
