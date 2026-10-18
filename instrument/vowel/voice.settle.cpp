// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void voice(const VOWEL::Singer &singer, VOWEL::Throat &throat) {
  const Float *rows = singer.rows;
  CORE::GLOTTIS::settle(
    throat.source, rows[VOWEL::TENSION], rows[VOWEL::BREATH], singer.rate);
  throat.vibrato.hertz = rows[VOWEL::RATE];
  throat.vibrato.depth = rows[VOWEL::DEPTH];
  throat.vibrato.fade = rows[VOWEL::DELAY];
  CORE::MODULATOR::settle(throat.vibrato, singer.pace);
  throat.pan = CORE::STEREO::pan(throat.place * rows[VOWEL::WIDTH]);
  CORE::MODULATOR::settle(throat.wobble, VOWEL::WOBBLE::TIME, singer.pace);
  CORE::MODULATOR::settle(throat.jitter, VOWEL::JITTER::TIME, singer.pace);
}

}  // namespace

void SOUND::PLUGINS::VOWEL::seat(Singer &singer, Whole at) {
  Throat &throat = singer.throats[at];
  const CORE::NOISE::Register state =
    CORE::NOISE::SEED ^ (CORE::NOISE::Register(at + 1) * SCATTER);
  CORE::GLOTTIS::seed(throat.source, state);
  CORE::MODULATOR::Seed seed = state * SCATTER;
  throat.place = CORE::MODULATOR::draw(seed);
  CORE::MODULATOR::seed(throat.wobble, seed);
  CORE::MODULATOR::seed(throat.jitter, seed * SCATTER);
}

void SOUND::PLUGINS::VOWEL::settle(Singer &singer) {
  const Float *rows = singer.rows;
  singer.gain = rows[GAIN];
  singer.envelope = CORE::ENVELOPE::ADSR::create(
    rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]);
  singer.envelope.depth = CORE::ENVELOPE::FULL;
  CORE::ENVELOPE::shape(singer.envelope, singer.rate);
  singer.shift = CORE::PHASE::ratio(rows[SEX] * SPAN);
  singer.pole = CORE::MODULATOR::pole(TRACT::TIME, singer.pace);
  singer.motion.hertz = rows[MOTION];
  CORE::MODULATOR::settle(singer.motion, singer.pace);
  allot(singer);
  aim(singer);
  for (Throat &throat : singer.throats) ::voice(singer, throat);
}
