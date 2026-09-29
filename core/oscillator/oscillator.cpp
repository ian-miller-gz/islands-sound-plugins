// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr Float TWICE = 2.0f;
constexpr Float HALF = 0.5f;
constexpr Float SLOPE = 4.0f;

using Shape = auto (*)(const OSCILLATOR::Oscillator &) -> Float;
using Naive = auto (*)(OSCILLATOR::Wheel, OSCILLATOR::Wheel) -> Float;

auto sawed(OSCILLATOR::Wheel phase, OSCILLATOR::Wheel) -> Float {
  return OSCILLATOR::NAIVE::saw(phase);
}

auto folded(OSCILLATOR::Wheel phase, OSCILLATOR::Wheel) -> Float {
  return OSCILLATOR::NAIVE::triangle(phase);
}

constexpr Shape SHAPES[] = {
  OSCILLATOR::saw, OSCILLATOR::pulse, OSCILLATOR::triangle};
constexpr Naive NAIVES[] = {sawed, OSCILLATOR::NAIVE::pulse, folded};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == OSCILLATOR::WAVES);
static_assert(sizeof(NAIVES) / sizeof(NAIVES[0]) == OSCILLATOR::WAVES);

auto chosen(Whole wave) -> Whole {
  return wave < OSCILLATOR::WAVES ? wave : OSCILLATOR::SAW;
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::settle(
  Oscillator &oscillator, Float hertz, Float width, Whole rate) {
  const Float top = Float(rate) * HALF;
  const Float held = hertz < 0 ? 0 : hertz > top ? top : hertz;
  const Float wide = width < NARROWEST ? NARROWEST
                     : width > WIDEST  ? WIDEST
                                       : width;
  oscillator.step = PHASE::step(held, rate);
  oscillator.edge = Wheel(wide * PHASE::TURN);
}

void SOUND::PLUGINS::CORE::OSCILLATOR::reset(
  Oscillator &oscillator, Wheel phase) {
  oscillator.phase = phase;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::shaped(
  const Oscillator &oscillator, Whole wave) -> Float {
  return ::SHAPES[::chosen(wave)](oscillator);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::tick(Oscillator &oscillator, Whole wave)
  -> Float {
  const Float value = shaped(oscillator, wave);
  oscillator.phase += oscillator.step;
  return value;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::NAIVE::saw(Wheel phase) -> Float {
  return PHASE::fraction(phase) * TWICE - UNIT;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::NAIVE::pulse(Wheel phase, Wheel edge)
  -> Float {
  return phase < edge ? UNIT : -UNIT;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::NAIVE::triangle(Wheel phase) -> Float {
  const Float centred = PHASE::fraction(phase) - HALF;
  return UNIT - SLOPE * (centred < 0 ? -centred : centred);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::NAIVE::shaped(
  Wheel phase, Wheel edge, Whole wave) -> Float {
  return ::NAIVES[::chosen(wave)](phase, edge);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::NAIVE::tick(
  Oscillator &oscillator, Whole wave) -> Float {
  const Float value = shaped(oscillator.phase, oscillator.edge, wave);
  oscillator.phase += oscillator.step;
  return value;
}
