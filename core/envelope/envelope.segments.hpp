// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::ENVELOPE {

constexpr Whole SEGMENTS = 4;
constexpr Whole HOLD = 2;
constexpr Whole RESTED = SEGMENTS;
constexpr Float TOP = 99.0f;
constexpr Whole RATES = 100;

struct Segments {
  Float rates[SEGMENTS] = {TOP, TOP, TOP, TOP};
  Float levels[SEGMENTS] = {TOP, TOP, TOP, 0};
  Whole curve = EXPONENTIAL;
  Float depth = 0;
  Float steps[SEGMENTS] = {};
  Float targets[SEGMENTS] = {};
};

struct Walk {
  Float level = 0;
  Whole segment = RESTED;
  Float scale = FULL;
};

auto seconds(Float rate) -> Float;
auto loudness(Float level, Whole curve) -> Float;

void shape(Segments &segments, Whole rate);
void strike(Walk &walk, const Segments &segments, Float velocity);
void lift(Walk &walk);
auto sounding(const Walk &walk) -> Flag;
auto tick(Walk &walk, const Segments &segments) -> Float;

}  // namespace SOUND::PLUGINS::CORE::ENVELOPE
