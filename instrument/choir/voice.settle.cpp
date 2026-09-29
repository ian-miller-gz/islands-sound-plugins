// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float FULL = 1.0f;
constexpr Float OPEN = 0.6f;
constexpr Float SPAN = 400.0f;
constexpr Float FAN = 200.0f;
constexpr Float PACE = 0.2f;
constexpr Float WIDTH = 0.8f;

auto panned(Float pan) -> CHOIR::Stereo {
  return {pan > 0 ? FULL - pan : FULL, pan < 0 ? FULL + pan : FULL};
}

void seat(const CHOIR::Choir &choir, CHOIR::Singer &singer, Float sex) {
  const Float *rows = choir.rows;
  const Float fan = singer.seat.shift * rows[CHOIR::SPREAD] * ::FAN;
  singer.shift = sex * CORE::PHASE::ratio(fan);
  singer.pan = ::panned(singer.seat.pan * ::WIDTH);
  CORE::GLOTTIS::settle(singer.source, ::OPEN, rows[CHOIR::BREATH]);
  const Float pace = FULL + singer.seat.pace * ::PACE;
  singer.vibrato.hertz = rows[CHOIR::RATE] * pace;
  singer.vibrato.depth = rows[CHOIR::DEPTH];
  CORE::MODULATOR::settle(singer.vibrato, choir.rate / CHOIR::CONTROL);
  CORE::FILTER::settle(singer.formant, choir.place, singer.shift, choir.rate);
}

void envelope(CHOIR::Choir &choir) {
  const Float *rows = choir.rows;
  choir.envelope = CORE::ENVELOPE::ADSR::create(
    rows[CHOIR::ATTACK], rows[CHOIR::DECAY], rows[CHOIR::SUSTAIN],
    rows[CHOIR::RELEASE]);
  choir.envelope.depth = CORE::ENVELOPE::FULL;
  choir.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(choir.envelope, choir.rate);
}

}  // namespace

void SOUND::CHOIR::settle(Choir &choir) {
  const Float *rows = choir.rows;
  choir.gain = rows[GAIN];
  choir.scatter = rows[SCATTER] * Float(choir.rate);
  place(choir);
  choir.motion.hertz = rows[MOTION];
  CORE::MODULATOR::settle(choir.motion, choir.rate / CONTROL);
  ::envelope(choir);
  const Float sex = CORE::PHASE::ratio(rows[SEX] * ::SPAN);
  for (Part &part : choir.parts)
    for (Singer &singer : part.singers) ::seat(choir, singer, sex);
  settle(choir.ensemble, choir.room, rows, choir.rate);
}
