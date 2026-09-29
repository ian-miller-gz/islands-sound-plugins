// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr Float TWICE = 2.0f;
constexpr Float THIRD = 1.0f / 3.0f;
constexpr Float CORNER = 4.0f;
constexpr OSCILLATOR::Wheel CREST = OSCILLATOR::Wheel(1) << 31;

auto cubed(Float value) -> Float { return value * value * value * THIRD; }

}  // namespace

auto SOUND::PLUGINS::CORE::OSCILLATOR::blep(Wheel phase, Wheel step) -> Float {
  if (step == 0) return 0;
  const Wheel left = Wheel(0) - phase;
  if (phase < step) {
    const Float x = Float(phase) / Float(step);
    return x * TWICE - x * x - UNIT;
  }
  if (left > step) return 0;
  const Float x = -Float(left) / Float(step);
  return x * x + x * TWICE + UNIT;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::blamp(Wheel phase, Wheel step) -> Float {
  if (step == 0) return 0;
  const Wheel left = Wheel(0) - phase;
  if (phase < step) return ::cubed(UNIT - Float(phase) / Float(step));
  if (left > step) return 0;
  return ::cubed(UNIT - Float(left) / Float(step));
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::saw(const Oscillator &oscillator)
  -> Float {
  return NAIVE::saw(oscillator.phase) - blep(oscillator.phase, oscillator.step);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::pulse(const Oscillator &oscillator)
  -> Float {
  const Wheel falling = oscillator.phase - oscillator.edge;
  return NAIVE::pulse(oscillator.phase, oscillator.edge) +
         blep(oscillator.phase, oscillator.step) -
         blep(falling, oscillator.step);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::triangle(const Oscillator &oscillator)
  -> Float {
  const Float span = PHASE::fraction(oscillator.step) * CORNER;
  const Float low = blamp(oscillator.phase, oscillator.step);
  const Float high = blamp(oscillator.phase - ::CREST, oscillator.step);
  return NAIVE::triangle(oscillator.phase) + span * (low - high);
}
