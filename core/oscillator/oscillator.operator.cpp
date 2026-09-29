// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdint>

#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float HALF = 0.5f;

auto offset(Float radians) -> OSCILLATOR::Wheel {
  const Float turns = radians / OSCILLATOR::TAU;
  const Float part = turns - std::floor(turns);
  return OSCILLATOR::Wheel(std::uint64_t(part * PHASE::TURN));
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::settle(
  Operator &unit, Float hertz, Float feedback, Whole rate) {
  const Float top = Float(rate) * HALF;
  unit.step = PHASE::step(hertz < 0 ? 0 : hertz > top ? top : hertz, rate);
  unit.feedback = feedback;
}

void SOUND::PLUGINS::CORE::OSCILLATOR::reset(Operator &unit, Wheel phase) {
  unit.phase = phase;
  for (Float &past : unit.history) past = 0;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::tick(
  Operator &unit, const Table &table, Float modulation, Float level) -> Float {
  const Float looped =
    unit.feedback * (unit.history[0] + unit.history[1]) * HALF;
  const Wheel at = unit.phase + ::offset(modulation + looped);
  const Float value = sine(table, at) * level;
  unit.history[1] = unit.history[0];
  unit.history[0] = value;
  unit.phase += unit.step;
  return value;
}
