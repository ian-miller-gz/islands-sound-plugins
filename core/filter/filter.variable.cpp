// SPDX-License-Identifier: AGPL-3.0-or-later
#include "filter.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float SMOOTH = 1.41421356f;
constexpr Float RINGING = 0.02f;

struct Mix {
  Float direct;
  Float damped;
  Float band;
  Float low;
};

constexpr Mix MIXES[] = {
  {0, 0, 0, 1}, {1, -1, 0, -1}, {0, 0, 1, 0}, {1, -1, 0, 0}};
constexpr Whole MODES = sizeof(MIXES) / sizeof(MIXES[0]);

}  // namespace

void SOUND::CORE::FILTER::settle(
  Variable &variable, Whole kind, Float cutoff, Float emphasis, Whole rate) {
  const Float g = warped(cutoff, rate);
  const Float k = SMOOTH - bounded(emphasis) * (SMOOTH - RINGING);
  const Mix &mix = MIXES[kind < MODES ? kind : LOW];
  variable.damping = k;
  variable.weights[0] = ONE / (ONE + g * (g + k));
  variable.weights[1] = g * variable.weights[0];
  variable.weights[2] = g * variable.weights[1];
  variable.mix[0] = mix.direct;
  variable.mix[1] = mix.damped * k + mix.band;
  variable.mix[2] = mix.low;
}

auto SOUND::CORE::FILTER::split(Variable &variable, Float in) -> Taps {
  const Float *weights = variable.weights;
  Float *states = variable.states;
  const Float lead = in - states[1];
  const Float band = weights[0] * states[0] + weights[1] * lead;
  const Float low = states[1] + weights[1] * states[0] + weights[2] * lead;
  states[0] = TWO * band - states[0];
  states[1] = TWO * low - states[1];
  const Float high = in - variable.damping * band - low;
  return {low, band, high, low + high};
}

auto SOUND::CORE::FILTER::tick(Variable &variable, Float in) -> Float {
  const Taps taps = split(variable, in);
  const Float *mix = variable.mix;
  return mix[0] * in + mix[1] * taps.band + mix[2] * taps.low;
}
