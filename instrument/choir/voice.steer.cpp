// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;
constexpr Float EFFORT = 0.5f;
constexpr Float JITTERED = 5.0f;

auto aimed(
  const CHOIR::Choir &choir, const CORE::VOICE::Note &note,
  Float glided) -> CORE::FILTER::Tract {
  const Float *rows = choir.rows;
  CORE::FILTER::Tract tract;
  CORE::FILTER::aim(
    tract, CHOIR::cast(choir, note), rows[CHOIR::VOWEL], rows[CHOIR::TARGET],
    choir.part);
  const Float effort = note.velocity * ::EFFORT;
  CORE::FILTER::bend(tract, CORE::PHASE::hertz(glided), effort);
  return tract;
}

auto cents(const CHOIR::Choir &choir, CHOIR::Singer &singer) -> Float {
  const Float *rows = choir.rows;
  const Float vibrato = CORE::MODULATOR::tick(singer.vibrato);
  const Float wobble = CORE::MODULATOR::tick(singer.wobble);
  const Float jitter = CORE::MODULATOR::tick(singer.jitter) / ::JITTERED;
  const Float detune = singer.seat.detune * rows[CHOIR::DETUNE];
  return detune + vibrato + rows[CHOIR::DRIFT] * (wobble + jitter);
}

void shape(
  const CHOIR::Choir &choir, CHOIR::Singer &singer,
  const CORE::FILTER::Tract &tract, Flag leap) {
  CORE::FILTER::Formant &formant = singer.formant;
  if (leap) {
    CORE::FILTER::jump(formant, tract, singer.shift, choir.rate);
    return;
  }
  CORE::FILTER::glide(formant, tract, choir.pole, singer.shift, choir.rate);
}

}  // namespace

void SOUND::PLUGINS::CHOIR::place(Choir &choir) {
  choir.part = choir.rows[MORPH] * (FULL + choir.sway) * HALF;
}

void SOUND::PLUGINS::CHOIR::tune(
  Choir &choir, CORE::VOICE::Note &note, Part &part, Flag leap) {
  const Float glided = CORE::VOICE::tick(note, choir.allocator.glide);
  const CORE::FILTER::Tract tract = ::aimed(choir, note, glided);
  for (Singer &singer : part.singers) {
    const Float pitch = glided + ::cents(choir, singer) / CENT;
    CORE::GLOTTIS::tune(singer.source, CORE::PHASE::hertz(pitch), choir.rate);
    ::shape(choir, singer, tract, leap);
  }
}

void SOUND::PLUGINS::CHOIR::steer(Choir &choir) {
  choir.sway = CORE::MODULATOR::tick(choir.motion);
  place(choir);
  for (Whole at = 0; at < NOTES; ++at) {
    CORE::VOICE::Note &note = choir.allocator.notes[at];
    if (note.sounding) tune(choir, note, choir.parts[at], false);
  }
}
