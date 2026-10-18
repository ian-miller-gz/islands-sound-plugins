// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float FULL = 1;
constexpr Float SHIFT = 0.25f;
constexpr Float DRIVE = 1;
constexpr Float BASS = 0;
constexpr Whole STRIDE = 16;
constexpr Whole OVERSAMPLE = 2;

auto swept(const DCO::Synth &synth, Whole at, Float contour, Float lfo)
  -> Float {
  const Float *rows = synth.rows;
  const CORE::VOICE::Note &note = synth.allocator.notes[at];
  const Float sign = rows[DCO::POLARITY] > 0 ? -FULL : FULL;
  Float octaves =
    rows[DCO::KEYBOARD] * (note.current - DCO::REFERENCE) / OCTAVE;
  octaves += sign * rows[DCO::ENVELOPE] * contour + rows[DCO::WOBBLE] * lfo;
  octaves += rows[DCO::TOUCH] * note.velocity;
  octaves += SHIFT * rows[DCO::TRIM] * synth.trims[at].cutoff;
  return rows[DCO::CUTOFF] * std::exp2(octaves);
}

}  // namespace

auto SOUND::PLUGINS::DCO::filter(
  Synth &synth, Whole at, Float in, Float contour, Float lfo) -> Float {
  Voice &voice = synth.voices[at];
  if (CORE::BLOCK::due(voice.stride, ::STRIDE)) {
    const Float cutoff = ::swept(synth, at, contour, lfo);
    CORE::FILTER::settle(
      voice.transistor, cutoff, synth.rows[RESONANCE], ::DRIVE, ::BASS,
      ::OVERSAMPLE * synth.rate);
  }
  CORE::FILTER::Transistor &transistor = voice.transistor;
  return CORE::SHAPER::tick(voice.oversampler, in, [&transistor](Float sample) {
    return CORE::FILTER::tick(transistor, sample);
  });
}
