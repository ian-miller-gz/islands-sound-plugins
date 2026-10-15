// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float BASS = 0;
constexpr Whole STRIDE = 16;

auto doubled(const DUO::Synth &synth) -> Whole {
  return synth.rate * DUO::OVERSAMPLING;
}

struct Pass {
  void (*settle)(DUO::Synth &synth, Float cutoff);
  auto (*tick)(DUO::Synth &synth, Float in) -> Float;
};

constexpr Pass PASSES[] = {
  {[](DUO::Synth &synth, Float cutoff) {
     CORE::FILTER::settle(
       synth.variable, CORE::FILTER::LOW, cutoff, synth.rows[DUO::RESONANCE],
       synth.saturation, ::doubled(synth));
   },
   [](DUO::Synth &synth, Float in) {
     return CORE::FILTER::tick(synth.variable, synth.gain * in) / synth.gain;
   }},
  {[](DUO::Synth &synth, Float cutoff) {
     CORE::FILTER::settle(
       synth.transistor, cutoff, synth.rows[DUO::RESONANCE], synth.gain, ::BASS,
       ::doubled(synth));
   },
   [](DUO::Synth &synth, Float in) {
     return CORE::FILTER::tick(synth.transistor, in);
   }}};
static_assert(std::size(PASSES) == std::size(DUO::SLOPES));

}  // namespace

void SOUND::PLUGINS::DUO::steer(Synth &synth, const Modulation &modulation) {
  if (!CORE::BLOCK::due(synth.stride, ::STRIDE)) return;
  const Float *rows = synth.rows;
  Float octaves = rows[TRACKING] * (modulation.key - REFERENCE) / ::OCTAVE;
  octaves += rows[AMOUNT] * modulation.contour + rows[TOUCH] * synth.velocity;
  octaves += rows[WOBBLE] * modulation.lfo + rows[SCATTER] * modulation.random;
  ::PASSES[synth.slope].settle(synth, rows[CUTOFF] * std::exp2(octaves));
}

auto SOUND::PLUGINS::DUO::filter(Synth &synth, Float in) -> Float {
  const Float passed = CORE::FILTER::tick(synth.highpass, in);
  const auto tick = ::PASSES[synth.slope].tick;
  const auto shape = [&synth, tick](Float sample) {
    return tick(synth, sample);
  };
  return CORE::SHAPER::tick(synth.oversampler, passed, shape);
}
