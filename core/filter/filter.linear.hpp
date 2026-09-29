// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::FILTER {

enum Kind : Whole { LOW, HIGH, BAND, NOTCH, PEAK, LOWSHELF, HIGHSHELF, KINDS };

constexpr Float LOWEST = 10.0f;
constexpr Float CEILING = 0.45f;
constexpr Float FLAT = 0.70710678f;
constexpr Whole FEEDS = 3;
constexpr Whole BACKS = 2;
constexpr Whole WEIGHTS = 3;
constexpr Whole INTEGRATORS = 2;

auto bounded(Float cutoff, Whole rate) -> Float;
auto bounded(Float emphasis) -> Float;
auto warped(Float cutoff, Whole rate) -> Float;

struct Pole {
  Float gain = 0;
  Float state = 0;
  Float direct = 0;
  Float low = 1;
};

void settle(Pole &pole, Whole kind, Float cutoff, Whole rate);
auto tick(Pole &pole, Float in) -> Float;

struct Biquad {
  Float feeds[FEEDS] = {1, 0, 0};
  Float backs[BACKS] = {0, 0};
  Float states[BACKS] = {0, 0};
};

void settle(
  Biquad &biquad, Whole kind, Float cutoff, Float q, Float gain, Whole rate);
auto tick(Biquad &biquad, Float in) -> Float;

struct Taps {
  Float low = 0;
  Float band = 0;
  Float high = 0;
  Float notch = 0;
};

struct Variable {
  Float damping = 0;
  Float weights[WEIGHTS] = {0, 0, 0};
  Float states[INTEGRATORS] = {0, 0};
  Float mix[WEIGHTS] = {0, 0, 1};
};

void settle(
  Variable &variable, Whole kind, Float cutoff, Float emphasis, Whole rate);
auto split(Variable &variable, Float in) -> Taps;
auto tick(Variable &variable, Float in) -> Float;

}  // namespace SOUND::PLUGINS::CORE::FILTER
