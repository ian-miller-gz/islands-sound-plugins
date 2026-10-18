// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float WOBBLE = 2;
constexpr Float CENT = 100;

}  // namespace

void SOUND::PLUGINS::LADDER::steer(
  Synth &synth, Float key, Float wheel, Float contour) {
  if (!CORE::BLOCK::due(synth.stride, STRIDE)) return;
  const Float *rows = synth.rows;
  synth.stray = CORE::MODULATOR::tick(synth.drift) * rows[DRIFT] / ::CENT;
  Float octaves = synth.tracking * (key - REFERENCE) / ::OCTAVE;
  octaves +=
    synth.stray + rows[AMOUNT] * contour + rows[TOUCH] * synth.velocity;
  if (rows[FILTER] > 0) octaves += wheel * ::WOBBLE;
  CORE::FILTER::settle(
    synth.transistor, rows[CUTOFF] * std::exp2(octaves), rows[EMPHASIS],
    rows[DRIVE], rows[BASS], synth.rate * OVERSAMPLING);
}

auto SOUND::PLUGINS::LADDER::filter(Synth &synth, Float in) -> Float {
  const auto shape = [&synth](Float sample) {
    return CORE::FILTER::tick(synth.transistor, sample);
  };
  return CORE::SHAPER::tick(synth.oversampler, in, shape);
}
