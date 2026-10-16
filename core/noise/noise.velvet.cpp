// SPDX-License-Identifier: AGPL-3.0-or-later
#include "noise.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole SHIFT = 1;

void place(NOISE::Velvet &velvet) {
  const Float drawn = NOISE::tick(velvet.white);
  velvet.sign = drawn < 0 ? -NOISE::EDGE : NOISE::EDGE;
  velvet.spot = velvet.white.state << SHIFT;
  velvet.struck = false;
}

}  // namespace

void SOUND::PLUGINS::CORE::NOISE::seed(Velvet &velvet, Register state) {
  seed(velvet.white, state);
  ::place(velvet);
}

void SOUND::PLUGINS::CORE::NOISE::settle(
  Velvet &velvet, Float hertz, Whole rate) {
  velvet.step = PHASE::step(audible(hertz, rate), rate);
}

auto SOUND::PLUGINS::CORE::NOISE::tick(Velvet &velvet) -> Float {
  const PHASE::Wheel before = velvet.phase;
  velvet.phase += velvet.step;
  const Flag wrapped = velvet.phase < before;
  Float value = 0;
  if (!velvet.struck && (wrapped || velvet.phase >= velvet.spot)) {
    velvet.struck = true;
    value = velvet.sign;
  }
  if (wrapped) ::place(velvet);
  return value;
}
