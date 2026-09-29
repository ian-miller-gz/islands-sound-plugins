// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <limits>

#include "oscillator.wave.hpp"

namespace SOUND::CORE::OSCILLATOR {

constexpr Whole BITS = 11;
constexpr Whole SIZE = Whole(1) << BITS;
constexpr Whole STRIDE = SIZE + 1;
constexpr Whole SHIFT = std::numeric_limits<Wheel>::digits - BITS;
constexpr Wheel MASK = (Wheel(1) << SHIFT) - 1;
constexpr Float SLICE = Float(MASK) + 1.0f;
constexpr Float TAU = 6.2831853f;
constexpr Whole SINGLE = 1;

struct Table {
  Whole frames = 0;
  Vector<Float> values;
};

void build(Table &table, Whole frames);
void build(Table &table);
void draw(Table &table, Whole frame, const Float *partials, Whole count);
auto read(const Table &table, Whole frame, Wheel phase) -> Float;
auto morph(const Table &table, Float position, Wheel phase) -> Float;
auto sine(const Table &table, Wheel phase) -> Float;

constexpr Whole HISTORY = 2;

struct Operator {
  Wheel phase = 0;
  Wheel step = 0;
  Float feedback = 0;
  Float history[HISTORY] = {0, 0};
};

void settle(Operator &unit, Float hertz, Float feedback, Whole rate);
void reset(Operator &unit, Wheel phase);
auto tick(Operator &unit, const Table &table, Float modulation, Float level)
  -> Float;

}  // namespace SOUND::CORE::OSCILLATOR
