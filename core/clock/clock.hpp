// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::CORE::CLOCK {

constexpr Float MINUTE = 60.0f;
constexpr Float SLOWEST = 1.0f;
constexpr Float LOOSEST = 0.9f;

struct Clock {
  Whole rate = 0;
  Float frames = 0;
  Float swing = 0;
  Float left = 0;
  Whole step = 0;
  Flag running = false;
};

struct Edge {
  Whole offset = 0;
  Whole step = 0;
};

auto frames(Float tempo, Float division, Whole rate) -> Float;
void settle(Clock &clock, Float tempo, Float division, Float swing, Whole rate);
auto length(const Clock &clock, Whole step) -> Float;
auto position(const Clock &clock) -> Float;

void run(Clock &clock);
void stop(Clock &clock);
void reset(Clock &clock);

auto advance(Clock &clock, Whole frames, Edge *edges, Whole room) -> Whole;

}  // namespace SOUND::CORE::CLOCK
