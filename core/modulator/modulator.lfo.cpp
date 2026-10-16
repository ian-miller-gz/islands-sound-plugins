// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float TAU = 6.2831853f;
constexpr Float DOUBLE = 2.0f;
constexpr PHASE::Wheel HALF = 0x80000000u;
constexpr PHASE::Wheel QUARTER = 0x40000000u;
constexpr PHASE::Wheel TROUGH = HALF + QUARTER;
constexpr Whole CYCLE = Whole(PHASE::TURN);

auto sine(const MODULATOR::Lfo &, PHASE::Wheel phase) -> Float {
  return std::sin(TAU * PHASE::fraction(phase));
}

auto triangle(const MODULATOR::Lfo &, PHASE::Wheel phase) -> Float {
  const Float ramp = PHASE::ramp(phase - QUARTER);
  return MODULATOR::FULL - DOUBLE * (ramp < 0 ? -ramp : ramp);
}

auto saw(const MODULATOR::Lfo &, PHASE::Wheel phase) -> Float {
  return PHASE::ramp(phase);
}

auto square(const MODULATOR::Lfo &, PHASE::Wheel phase) -> Float {
  return phase < HALF ? MODULATOR::FULL : -MODULATOR::FULL;
}

auto sample(const MODULATOR::Lfo &lfo, PHASE::Wheel) -> Float {
  return lfo.held;
}

auto wander(const MODULATOR::Lfo &lfo, PHASE::Wheel) -> Float {
  return lfo.value;
}

using Shape = auto (*)(const MODULATOR::Lfo &, PHASE::Wheel) -> Float;

constexpr Shape SHAPES[MODULATOR::WAVES] = {sine,   triangle, saw,
                                            square, sample,   wander};

constexpr PHASE::Wheel ORIGINS[MODULATOR::WAVES] = {TROUGH, TROUGH, HALF,
                                                    0,      0,      0};

auto placed(Float start) -> PHASE::Wheel {
  const Float place = start < 0 ? 0 : start >= MODULATOR::FULL ? 0 : start;
  return PHASE::Wheel(place * PHASE::TURN);
}

auto warped(Whole split, PHASE::Wheel phase) -> PHASE::Wheel {
  const Whole at = phase;
  if (at < split) return PHASE::Wheel(at * HALF / split);
  return PHASE::Wheel(HALF + (at - split) * HALF / (CYCLE - split));
}

}  // namespace

void SOUND::PLUGINS::CORE::MODULATOR::reset(Lfo &lfo) {
  lfo.phase = ::placed(lfo.start);
  lfo.faded = 0;
  lfo.held = draw(lfo.seed);
}

auto SOUND::PLUGINS::CORE::MODULATOR::tick(Lfo &lfo) -> Float {
  lfo.faded += lfo.rise;
  if (lfo.faded > FULL) lfo.faded = FULL;
  lfo.value += (lfo.held - lfo.value) * lfo.glide;
  const Whole wave = lfo.wave < WAVES ? lfo.wave : SINE;
  const PHASE::Wheel origin = ::ORIGINS[wave];
  const PHASE::Wheel phase = ::warped(lfo.split, lfo.phase - origin) + origin;
  const Float out = ::SHAPES[wave](lfo, phase);
  const PHASE::Wheel before = lfo.phase;
  lfo.phase += lfo.step;
  if (lfo.phase < before) lfo.held = draw(lfo.seed);
  return out * lfo.depth * lfo.faded;
}
