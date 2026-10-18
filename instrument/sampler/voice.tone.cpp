// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LENGTH = 2;
constexpr Float KEY = 60;
constexpr Float BRIGHT = 6000;
constexpr Float DARK = 1500;
constexpr Float LOUD = 0.5f;
constexpr Float QUIET = 0.15f;
constexpr Float HALF = 0.5f;
constexpr Float OPEN = 0;

auto fallen(Float from, Float to, Float place) -> Float {
  const Float span = CORE::DYNAMICS::decibels(to / from);
  return from * CORE::DYNAMICS::gain(span * place);
}

auto voiced(CORE::OSCILLATOR::Oscillator &oscillator) -> Float {
  const Float saw = CORE::OSCILLATOR::shaped(oscillator, CORE::OSCILLATOR::SAW);
  const Float pulse =
    CORE::OSCILLATOR::tick(oscillator, CORE::OSCILLATOR::PULSE);
  return HALF * saw + HALF * pulse;
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::tone(Synth &synth) {
  const Whole count = Whole(::LENGTH * Float(synth.rate));
  Vector<Float> line(count);
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::settle(
    oscillator, CORE::PHASE::hertz(::KEY), CORE::OSCILLATOR::SQUARE,
    synth.rate);
  CORE::FILTER::Variable filter;
  for (Whole frame = 0; frame < count; ++frame) {
    const Float place = Float(frame) / Float(count);
    if (frame % STRIDE == 0)
      CORE::FILTER::settle(
        filter, CORE::FILTER::LOW, ::fallen(::BRIGHT, ::DARK, place), ::OPEN,
        synth.rate);
    const Float level = ::fallen(::LOUD, ::QUIET, place);
    line[frame] = level * CORE::FILTER::tick(filter, ::voiced(oscillator));
  }
  synth.clips.push_back({.name = CLIPS[0], .lanes = {line}, .ratio = 1});
}
