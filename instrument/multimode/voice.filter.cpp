// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float PEAK = 0.9f;

auto tap(const MULTIMODE::Synth &synth, MULTIMODE::Voice &voice, Float in)
  -> Float {
  const Float *rows = synth.rows;
  const CORE::FILTER::Taps taps = CORE::FILTER::split(voice.variable, in);
  if (rows[MULTIMODE::BAND] > 0) return taps.band;
  return taps.low + (taps.high - taps.low) * rows[MULTIMODE::MODE];
}

}  // namespace

void SOUND::PLUGINS::MULTIMODE::steer(
  Synth &synth, Voice &voice, Float cutoff) {
  const Float *rows = synth.rows;
  CORE::FILTER::settle(
    voice.variable, CORE::FILTER::LOW, cutoff, rows[RESONANCE] * ::PEAK,
    rows[SATURATION], synth.rate * OVERSAMPLING);
}

auto SOUND::PLUGINS::MULTIMODE::filter(Synth &synth, Voice &voice, Float in)
  -> Float {
  const auto shape = [&synth, &voice](Float sample) {
    return ::tap(synth, voice, sample);
  };
  return CORE::SHAPER::tick(voice.oversampler, in, shape);
}
