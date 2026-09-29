// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "modulator.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float TAU = 6.2831853f;
constexpr Float DOUBLE = 2.0f;
constexpr PHASE::Wheel HALF = 0x80000000u;
constexpr PHASE::Wheel QUARTER = 0x40000000u;

auto sine(const MODULATOR::Lfo &lfo) -> Float {
  return std::sin(TAU * PHASE::fraction(lfo.phase));
}

auto triangle(const MODULATOR::Lfo &lfo) -> Float {
  const Float ramp = PHASE::ramp(lfo.phase - QUARTER);
  return MODULATOR::FULL - DOUBLE * (ramp < 0 ? -ramp : ramp);
}

auto saw(const MODULATOR::Lfo &lfo) -> Float { return PHASE::ramp(lfo.phase); }

auto square(const MODULATOR::Lfo &lfo) -> Float {
  return lfo.phase < HALF ? MODULATOR::FULL : -MODULATOR::FULL;
}

auto sample(const MODULATOR::Lfo &lfo) -> Float { return lfo.held; }

auto wander(const MODULATOR::Lfo &lfo) -> Float { return lfo.value; }

using Shape = auto (*)(const MODULATOR::Lfo &) -> Float;

constexpr Shape SHAPES[MODULATOR::WAVES] = {sine,   triangle, saw,
                                            square, sample,   wander};

auto placed(Float start) -> PHASE::Wheel {
  const Float place = start < 0 ? 0 : start >= MODULATOR::FULL ? 0 : start;
  return PHASE::Wheel(place * PHASE::TURN);
}

}  // namespace

void SOUND::CORE::MODULATOR::reset(Lfo &lfo) {
  lfo.phase = ::placed(lfo.start);
  lfo.faded = 0;
  lfo.held = draw(lfo.seed);
}

auto SOUND::CORE::MODULATOR::tick(Lfo &lfo) -> Float {
  lfo.faded += lfo.rise;
  if (lfo.faded > FULL) lfo.faded = FULL;
  lfo.value += (lfo.held - lfo.value) * lfo.glide;
  const Float out = ::SHAPES[lfo.wave < WAVES ? lfo.wave : SINE](lfo);
  const PHASE::Wheel before = lfo.phase;
  lfo.phase += lfo.step;
  if (lfo.phase < before) lfo.held = draw(lfo.seed);
  return out * lfo.depth * lfo.faded;
}
