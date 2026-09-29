// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

void SOUND::PLUGINS::SPRING::build(Spring &spring, Float seconds, Whole rate) {
  CORE::LINE::build(spring.line, Whole(std::ceil(seconds * Float(rate))) + 1);
  for (auto &stage : spring.stages) CORE::REVERB::build(stage, Whole(STEP));
}

void SOUND::PLUGINS::SPRING::settle(
  Spring &spring, Float seconds, Float chirp, Float decay, Whole rate) {
  spring.delay = seconds * Float(rate);
  const Float gain = CORE::REVERB::decay(spring.delay, decay, rate);
  CORE::LINE::settle(spring.loop, gain, BRIGHT, rate);
  for (auto &stage : spring.stages) CORE::REVERB::settle(stage, STEP, -chirp);
}

auto SOUND::PLUGINS::SPRING::tick(Spring &spring, Float in) -> Float {
  const Float out = CORE::LINE::read(spring.line, spring.delay);
  Float fed = in + spring.loop.feedback * CORE::LINE::damp(spring.loop, out);
  for (auto &stage : spring.stages) fed = CORE::REVERB::tick(stage, fed);
  CORE::LINE::write(spring.line, fed);
  return out;
}
