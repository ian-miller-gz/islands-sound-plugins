// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "shaper.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float HALF = 0.5f;
constexpr Float FEWEST = 1.0f;
constexpr Float MOST = 24.0f;

}  // namespace

void SOUND::PLUGINS::CORE::SHAPER::settle(
  Crusher &crusher, Float bits, Float hertz, Whole rate) {
  const Float depth = bits < FEWEST ? FEWEST : bits > MOST ? MOST : bits;
  const Float stride = hertz / Float(rate);
  crusher.step = std::pow(TWO, ONE - depth);
  crusher.stride = stride < 0 ? 0 : stride > ONE ? ONE : stride;
}

auto SOUND::PLUGINS::CORE::SHAPER::tick(Crusher &crusher, Float in) -> Float {
  crusher.phase += crusher.stride;
  if (crusher.phase < ONE) return crusher.held;
  crusher.phase -= ONE;
  crusher.held = std::floor(in / crusher.step + HALF) * crusher.step;
  return crusher.held;
}
