// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float DRIVE = 1;
constexpr Float BASS = 0;
constexpr Whole STRIDE = 16;
constexpr Whole OVERSAMPLE = 2;

auto swept(const MONO::Synth &synth, Float key, Float lfo, Float contour)
  -> Float {
  const Float *rows = synth.rows;
  const Float velocity = synth.allocator.notes[0].velocity;
  Float octaves = rows[MONO::KEYBOARD] * (key - MONO::REFERENCE) / OCTAVE;
  octaves += rows[MONO::ENVELOPE] * contour + rows[MONO::MODULATION] * lfo;
  octaves += rows[MONO::TOUCH] * velocity;
  octaves += rows[MONO::DRIFT] * synth.wander.value / CENT;
  return rows[MONO::CUTOFF] * std::exp2(octaves);
}

}  // namespace

auto SOUND::PLUGINS::MONO::filter(
  Synth &synth, Float in, Float key, Float lfo, Float contour) -> Float {
  if (CORE::BLOCK::due(synth.stride, ::STRIDE)) {
    const Float cutoff = ::swept(synth, key, lfo, contour);
    CORE::FILTER::settle(
      synth.transistor, cutoff, synth.rows[RESONANCE], ::DRIVE, ::BASS,
      ::OVERSAMPLE * synth.rate);
  }
  CORE::FILTER::Transistor &transistor = synth.transistor;
  return CORE::SHAPER::tick(synth.oversampler, in, [&transistor](Float sample) {
    return CORE::FILTER::tick(transistor, sample);
  });
}
