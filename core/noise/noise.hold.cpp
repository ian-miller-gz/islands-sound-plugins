// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "noise.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float HALF = 0.5f;
constexpr Float UNIT = 1.0f;
constexpr Float TAU = 6.2831853f;

auto audible(Float hertz, Whole rate) -> Float {
  const Float top = Float(rate) * HALF;
  return hertz < 0 ? 0 : hertz > top ? top : hertz;
}

}  // namespace

void SOUND::PLUGINS::CORE::NOISE::settle(Hold &hold, Float hertz, Whole rate) {
  hold.step = PHASE::step(::audible(hertz, rate), rate);
}

void SOUND::PLUGINS::CORE::NOISE::reset(Hold &hold) { hold.phase = 0; }

auto SOUND::PLUGINS::CORE::NOISE::tick(Hold &hold, Float input) -> Float {
  if (hold.phase < hold.step) hold.value = input;
  hold.phase += hold.step;
  return hold.value;
}

void SOUND::PLUGINS::CORE::NOISE::settle(
  Burst &burst, Float seconds, Float hertz, Whole rate) {
  const Float frames = seconds * Float(rate);
  burst.decay = frames <= UNIT ? 0 : std::exp(-UNIT / frames);
  const Float turn = TAU * ::audible(hertz, rate) / Float(rate);
  burst.colour = UNIT - std::exp(-turn);
}

void SOUND::PLUGINS::CORE::NOISE::strike(Burst &burst, Float level) {
  burst.level = level;
  burst.low = 0;
}

auto SOUND::PLUGINS::CORE::NOISE::tick(Burst &burst) -> Float {
  if (burst.level == 0) return 0;
  burst.low += burst.colour * (tick(burst.white) - burst.low);
  const Float value = burst.low * burst.level;
  burst.level *= burst.decay;
  return value;
}
