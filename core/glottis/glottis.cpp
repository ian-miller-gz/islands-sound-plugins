// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "glottis.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float PI = 3.14159265f;
constexpr Float HALF = 0.5f;
constexpr Float CLOSED = 0.25f;

auto bounded(Float value, Float least, Float most) -> Float {
  return value < least ? least : value > most ? most : value;
}

auto slope(Float time, Float open) -> Float {
  const Float peak = open * GLOTTIS::SKEW;
  if (time < peak) return HALF * PI / peak * std::sin(PI * time / peak);
  if (time >= open) return 0;
  const Float fall = open - peak;
  return -HALF * PI / fall * std::sin(HALF * PI * (time - peak) / fall);
}

void draw(Float *values, Float open) {
  Float most = 0;
  for (Whole at = 0; at < OSCILLATOR::SIZE; ++at) {
    values[at] = ::slope(Float(at) / Float(OSCILLATOR::SIZE), open);
    most = std::fmax(most, std::fabs(values[at]));
  }
  if (most > 0)
    for (Whole at = 0; at < OSCILLATOR::SIZE; ++at) values[at] /= most;
  values[OSCILLATOR::SIZE] = values[0];
}

}  // namespace

auto SOUND::PLUGINS::CORE::GLOTTIS::quotient(Whole shape) -> Float {
  const Float span = WIDEST - CLOSEST;
  return CLOSEST + span * Float(shape) / Float(SHAPES - 1);
}

auto SOUND::PLUGINS::CORE::GLOTTIS::place(Float open) -> Float {
  const Float held = ::bounded(open, CLOSEST, WIDEST);
  return (held - CLOSEST) / (WIDEST - CLOSEST) * Float(SHAPES - 1);
}

void SOUND::PLUGINS::CORE::GLOTTIS::build(OSCILLATOR::Table &table) {
  OSCILLATOR::build(table, SHAPES);
  for (Whole shape = 0; shape < SHAPES; ++shape)
    ::draw(table.values.data() + shape * OSCILLATOR::STRIDE, quotient(shape));
}

void SOUND::PLUGINS::CORE::GLOTTIS::seed(
  Source &source, NOISE::Register state) {
  NOISE::seed(source.white, state);
}

void SOUND::PLUGINS::CORE::GLOTTIS::settle(
  Source &source, Float open, Float breath) {
  const Float held = ::bounded(open, CLOSEST, WIDEST);
  source.place = place(held);
  source.edge = PHASE::Wheel(held * PHASE::TURN);
  source.breath = ::bounded(breath, 0, FULL);
}

void SOUND::PLUGINS::CORE::GLOTTIS::tune(
  Source &source, Float hertz, Whole rate) {
  source.step = PHASE::step(hertz, rate);
}

void SOUND::PLUGINS::CORE::GLOTTIS::reset(Source &source) { source.phase = 0; }

auto SOUND::PLUGINS::CORE::GLOTTIS::tick(
  Source &source, const OSCILLATOR::Table &table) -> Float {
  const Float pulse = OSCILLATOR::morph(table, source.place, source.phase);
  const Float gate = source.phase < source.edge ? FULL : ::CLOSED;
  const Float noise = NOISE::tick(source.white) * gate;
  source.phase += source.step;
  return pulse + (noise - pulse) * source.breath;
}
