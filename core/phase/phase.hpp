// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::PHASE {

using Wheel = std::uint32_t;

constexpr Whole PITCHES = 128;
constexpr Float CONCERT = 440.0f;
constexpr Float ANCHOR = 69.0f;
constexpr Float OCTAVE = 12.0f;
constexpr Float CENTS = 1200.0f;
constexpr Float TURN = 4294967296.0f;
constexpr Float HALF = 2147483648.0f;

auto hertz(Float pitch) -> Float;
auto step(Float hertz, Whole rate) -> Wheel;
auto ratio(Float cents) -> Float;
auto fraction(Wheel phase) -> Float;
auto ramp(Wheel phase) -> Float;

struct Tuning {
  Whole rate = 0;
  Vector<Wheel> steps;
};

void tune(Tuning &tuning, Whole rate);
auto stepped(const Tuning &tuning, Whole pitch) -> Wheel;

}  // namespace SOUND::PLUGINS::CORE::PHASE
