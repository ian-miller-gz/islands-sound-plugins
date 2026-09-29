// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

void SOUND::SATURATOR::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  CORE::NOISE::Register seed = CORE::NOISE::SEED;
  for (auto &strip : effect.strips) {
    CORE::SHAPER::settle(strip.blocker, STILL, effect.rate);
    CORE::NOISE::seed(strip.hiss, seed++);
  }
}

void SOUND::SATURATOR::settle(Effect &effect) {
  effect.gain = CORE::DYNAMICS::gain(effect.rows[DRIVE]);
  effect.bias = effect.rows[BIAS];
  effect.rest = CORE::SHAPER::soft(effect.bias);
  effect.trim = UNITY / std::pow(effect.gain, TRIM);
  effect.hiss = FLOOR * effect.rows[HISS];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
  for (auto &strip : effect.strips) {
    CORE::FILTER::settle(
      strip.low, CORE::FILTER::HIGH, effect.rows[LOW], CORE::FILTER::FLAT, 0,
      effect.rate);
    CORE::FILTER::settle(
      strip.high, CORE::FILTER::LOW, effect.rows[HIGH], CORE::FILTER::FLAT, 0,
      effect.rate);
  }
}
