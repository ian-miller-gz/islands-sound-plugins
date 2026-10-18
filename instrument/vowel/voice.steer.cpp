// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;

auto cents(const VOWEL::Singer &singer, VOWEL::Throat &throat) -> Float {
  const Float vibrato = CORE::MODULATOR::tick(throat.vibrato);
  const Float wobble = CORE::MODULATOR::tick(throat.wobble);
  const Float jitter = CORE::MODULATOR::tick(throat.jitter);
  const Float wander = wobble + jitter / VOWEL::JITTER::SHARE;
  return vibrato + singer.rows[VOWEL::DRIFT] * wander;
}

}  // namespace

void SOUND::PLUGINS::VOWEL::aim(Singer &singer) {
  const Float *rows = singer.rows;
  const Float part = rows[MORPH] * (FULL + singer.sway) * HALF;
  CORE::FILTER::aim(singer.tract, rows[VOICE], rows[VOWEL], rows[TARGET], part);
}

void SOUND::PLUGINS::VOWEL::steer(Singer &singer) {
  singer.sway = CORE::MODULATOR::tick(singer.motion);
  aim(singer);
  for (Whole at = 0; at < THROATS; ++at)
    if (singer.allocator.notes[at].sounding) steer(singer, at, false);
}

void SOUND::PLUGINS::VOWEL::steer(Singer &singer, Whole at, Flag struck) {
  CORE::VOICE::Note &note = singer.allocator.notes[at];
  Throat &throat = singer.throats[at];
  const Float key = CORE::VOICE::tick(note, singer.allocator.glide);
  const Float hertz = CORE::PHASE::hertz(key + ::cents(singer, throat) / CENT);
  CORE::GLOTTIS::tune(throat.source, hertz, singer.rate);
  CORE::FILTER::Tract tract = singer.tract;
  CORE::FILTER::bend(tract, hertz, singer.rows[EFFORT] * note.velocity);
  if (struck) {
    CORE::FILTER::jump(throat.formant, tract, singer.shift, singer.rate);
    return;
  }
  CORE::FILTER::glide(
    throat.formant, tract, singer.pole, singer.shift, singer.rate);
}
