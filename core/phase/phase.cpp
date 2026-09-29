// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "phase.hpp"

auto SOUND::PLUGINS::CORE::PHASE::hertz(Float pitch) -> Float {
  return CONCERT * std::pow(2.0f, (pitch - ANCHOR) / OCTAVE);
}

auto SOUND::PLUGINS::CORE::PHASE::step(Float hertz, Whole rate) -> Wheel {
  if (rate == 0) return 0;
  const Float turns = hertz / Float(rate);
  return static_cast<Wheel>(turns * TURN);
}

auto SOUND::PLUGINS::CORE::PHASE::ratio(Float cents) -> Float {
  return std::pow(2.0f, cents / CENTS);
}

auto SOUND::PLUGINS::CORE::PHASE::fraction(Wheel phase) -> Float {
  return Float(phase) / TURN;
}

auto SOUND::PLUGINS::CORE::PHASE::ramp(Wheel phase) -> Float {
  return Float(static_cast<std::int32_t>(phase)) / HALF;
}

void SOUND::PLUGINS::CORE::PHASE::tune(Tuning &tuning, Whole rate) {
  tuning.rate = rate;
  tuning.steps.resize(PITCHES);
  for (Whole pitch = 0; pitch < PITCHES; ++pitch)
    tuning.steps[pitch] = step(hertz(Float(pitch)), rate);
}

auto SOUND::PLUGINS::CORE::PHASE::stepped(const Tuning &tuning, Whole pitch)
  -> Wheel {
  return pitch < tuning.steps.size() ? tuning.steps[pitch] : 0;
}
