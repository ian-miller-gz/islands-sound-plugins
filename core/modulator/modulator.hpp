// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../phase/phase.hpp"

namespace SOUND::PLUGINS::CORE::MODULATOR {

enum Wave : Whole { SINE, TRIANGLE, SAW, SQUARE, SAMPLE, RANDOM, WAVES };

using Seed = std::uint32_t;

constexpr Seed SEED = 0x9E3779B9u;
constexpr Float FULL = 1.0f;
constexpr Float WANDER = 0.57735027f;
constexpr Float EVEN = 0.5f;

struct Lfo {
  Whole wave = SINE;
  Float hertz = FULL;
  Float period = 0;
  Float depth = FULL;
  Float fade = 0;
  Float slew = 0;
  Float start = 0;
  Float skew = EVEN;
  PHASE::Wheel step = 0;
  Whole split = Whole(PHASE::HALF);
  Float rise = FULL;
  Float glide = FULL;
  PHASE::Wheel phase = 0;
  Float faded = FULL;
  Float held = 0;
  Float value = 0;
  Seed seed = SEED;
};

struct Slew {
  Float rise = 0;
  Float fall = 0;
  Float shape = 0;
  Float up = FULL;
  Float down = FULL;
  Float lift = FULL;
  Float drop = FULL;
  Float value = 0;
};

struct Smoother {
  Float time = 0;
  Float pole = FULL;
  Float value = 0;
};

struct Drift {
  Float leak = 0;
  Float gain = 0;
  Float value = 0;
  Seed seed = SEED;
};

auto delta(Float seconds, Whole rate) -> Float;
auto pole(Float seconds, Whole rate) -> Float;
auto draw(Seed &seed) -> Float;

void settle(Lfo &lfo, Whole rate);
void reset(Lfo &lfo);
auto tick(Lfo &lfo) -> Float;

void settle(Slew &slew, Whole rate);
void jump(Slew &slew, Float value);
auto tick(Slew &slew, Float in) -> Float;

void settle(Smoother &smoother, Whole rate);
void jump(Smoother &smoother, Float value);
auto tick(Smoother &smoother, Float target) -> Float;

void seed(Drift &drift, Seed state);
void settle(Drift &drift, Float seconds, Whole rate);
auto tick(Drift &drift) -> Float;

}  // namespace SOUND::PLUGINS::CORE::MODULATOR
