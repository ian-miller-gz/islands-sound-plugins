// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../noise/noise.hpp"
#include "../oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::CORE::GLOTTIS {

constexpr Whole SHAPES = 16;
constexpr Float CLOSEST = 0.3f;
constexpr Float WIDEST = 0.9f;
constexpr Float SKEW = 0.7f;
constexpr Float FULL = 1.0f;

struct Source {
  PHASE::Wheel phase = 0;
  PHASE::Wheel step = 0;
  PHASE::Wheel edge = 0;
  Float place = 0;
  Float breath = 0;
  NOISE::White white;
};

auto quotient(Whole shape) -> Float;
auto place(Float open) -> Float;

void build(OSCILLATOR::Table &table);
void seed(Source &source, NOISE::Register state);
void settle(Source &source, Float open, Float breath);
void tune(Source &source, Float hertz, Whole rate);
void reset(Source &source);
auto tick(Source &source, const OSCILLATOR::Table &table) -> Float;

}  // namespace SOUND::PLUGINS::CORE::GLOTTIS
