// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "modulator.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SHORTEST = 0.001f;
constexpr Float LONGEST = 10.0f;

auto held(Float value, Float least, Float most) -> Float {
  return value < least ? least : value > most ? most : value;
}

}  // namespace

void SOUND::PLUGINS::CORE::MODULATOR::seed(Drift &drift, Seed state) {
  drift.seed = state == 0 ? SEED : state;
}

void SOUND::PLUGINS::CORE::MODULATOR::settle(
  Drift &drift, Float seconds, Whole rate) {
  drift.leak = FULL - pole(::held(seconds, ::SHORTEST, ::LONGEST), rate);
  drift.gain = WANDER * std::sqrt(FULL - drift.leak * drift.leak);
}

auto SOUND::PLUGINS::CORE::MODULATOR::tick(Drift &drift) -> Float {
  const Float next = drift.leak * drift.value + drift.gain * draw(drift.seed);
  drift.value = ::held(next, -FULL, FULL);
  return drift.value;
}
