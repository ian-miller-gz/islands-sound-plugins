// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void open(CORE::DYNAMICS::Detector &gate, Whole rate) {
  gate.kind = CORE::DYNAMICS::PEAK;
  gate.attack = FUZZ::OPENING;
  gate.release = FUZZ::CLOSING;
  CORE::DYNAMICS::settle(gate, rate);
}

}  // namespace

void SOUND::PLUGINS::FUZZ::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  for (auto &strip : effect.strips) {
    ::open(strip.gate, effect.rate);
    CORE::SHAPER::settle(strip.blocker, STILL, effect.rate);
    CORE::FILTER::settle(strip.low, CORE::FILTER::LOW, LOWS, effect.rate);
    CORE::FILTER::settle(strip.high, CORE::FILTER::HIGH, HIGHS, effect.rate);
  }
}

void SOUND::PLUGINS::FUZZ::settle(Effect &effect) {
  effect.gain = GENTLEST * std::pow(HOTTEST / GENTLEST, effect.rows[FUZZ]);
  effect.bias = effect.rows[BIAS];
  effect.rest = CORE::SHAPER::fold(effect.bias);
  effect.highs = effect.rows[TONE];
  effect.lows = UNITY - effect.rows[TONE];
  effect.level = CORE::DYNAMICS::gain(effect.rows[LEVEL]);
}
