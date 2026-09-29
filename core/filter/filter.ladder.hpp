// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../shaper/shaper.hpp"
#include "filter.linear.hpp"

namespace SOUND::PLUGINS::CORE::FILTER {

constexpr Whole POLES = 4;
constexpr Whole STAGES = 4;
constexpr Whole LOOPS = 3;

struct Ladder {
  Pole poles[POLES];
  Float weights[POLES] = {0, 0, 0, 0};
  Float feedback = 0;
  Float scale = 1;
  Float drive = 1;
};

void settle(
  Ladder &ladder, Float cutoff, Float emphasis, Float drive, Whole rate);
auto tick(Ladder &ladder, Float in) -> Float;

struct Diode {
  Float betas[STAGES] = {};
  Float gammas[STAGES] = {};
  Float deltas[STAGES] = {};
  Float epsilons[STAGES] = {};
  Float sums[STAGES] = {};
  Float feedbacks[STAGES] = {};
  Float states[STAGES] = {};
  Float alpha = 0;
  Float feedback = 0;
  Float scale = 1;
  Float drive = 1;
};

void settle(
  Diode &diode, Float cutoff, Float emphasis, Float drive, Whole rate);
auto tick(Diode &diode, Float in) -> Float;

struct Sallen {
  Pole poles[LOOPS];
  Whole kind = LOW;
  Float feedback = 0;
  Float scale = 1;
  Float drive = 1;
};

void settle(
  Sallen &sallen, Whole kind, Float cutoff, Float emphasis, Float drive,
  Whole rate);
auto tick(Sallen &sallen, Float in) -> Float;

}  // namespace SOUND::PLUGINS::CORE::FILTER
