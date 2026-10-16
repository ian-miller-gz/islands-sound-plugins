// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;

auto shifted(const WHISPER::Mouth &mouth, Float glided) -> Float {
  const Float *rows = mouth.rows;
  const Float sex = rows[WHISPER::SEX] * WHISPER::SPAN;
  const Float key = glided - WHISPER::CENTRE;
  return CORE::PHASE::ratio(
    sex + rows[WHISPER::TRACKING] * key * WHISPER::CENT);
}

auto aimed(const WHISPER::Mouth &mouth) -> CORE::FILTER::Tract {
  const Float *rows = mouth.rows;
  CORE::FILTER::Tract tract;
  CORE::FILTER::aim(
    tract, rows[WHISPER::VOICE], rows[WHISPER::VOWEL], rows[WHISPER::TARGET],
    mouth.part);
  return tract;
}

}  // namespace

void SOUND::PLUGINS::WHISPER::place(Mouth &mouth) {
  mouth.part = mouth.rows[MORPH] * (::FULL + mouth.sway) * ::HALF;
}

void SOUND::PLUGINS::WHISPER::shape(Mouth &mouth, Whole at, Flag leap) {
  if (at >= HISSES) return;
  CORE::VOICE::Note &note = mouth.allocator.notes[at];
  Hiss &hiss = mouth.hisses[at];
  const Float glided = CORE::VOICE::tick(note, mouth.allocator.glide);
  const Float shift = ::shifted(mouth, glided);
  const CORE::FILTER::Tract tract = ::aimed(mouth);
  hiss.formant.breadth = mouth.rows[BREADTH];
  if (leap)
    CORE::FILTER::jump(hiss.formant, tract, shift, mouth.rate);
  else
    CORE::FILTER::glide(hiss.formant, tract, mouth.pole, shift, mouth.rate);
  hiss.makeup = CORE::FILTER::makeup(hiss.formant, mouth.rate);
  const Float hertz = CORE::PHASE::hertz(glided);
  CORE::GLOTTIS::tune(hiss.source, hertz, mouth.rate);
}

void SOUND::PLUGINS::WHISPER::steer(Mouth &mouth) {
  mouth.sway = CORE::MODULATOR::tick(mouth.motion);
  place(mouth);
  for (Whole at = 0; at < HISSES; ++at)
    if (mouth.allocator.notes[at].sounding) shape(mouth, at, false);
}
