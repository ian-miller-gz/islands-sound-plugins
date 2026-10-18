// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "noise.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float WIDEST = 2.0f * NOISE::EDGE;

auto reflected(Float value) -> Float {
  if (value > NOISE::EDGE) return WIDEST - value;
  if (value < -NOISE::EDGE) return -WIDEST - value;
  return value;
}

}  // namespace

void SOUND::PLUGINS::CORE::NOISE::seed(Walk &walk, Register state) {
  seed(walk.white, state);
}

void SOUND::PLUGINS::CORE::NOISE::settle(
  Walk &walk, Float hertz, Float step, Whole rate) {
  const Float held = audible(hertz, rate);
  walk.step = PHASE::step(held, rate);
  walk.reach = step < 0 ? 0 : step > ::WIDEST ? ::WIDEST : step;
  walk.pole = rate == 0 ? EDGE : EDGE - std::exp(-held / Float(rate));
}

auto SOUND::PLUGINS::CORE::NOISE::tick(Walk &walk) -> Float {
  if (walk.phase < walk.step)
    walk.target = ::reflected(walk.target + walk.reach * tick(walk.white));
  walk.phase += walk.step;
  walk.value += walk.pole * (walk.target - walk.value);
  return walk.value;
}
