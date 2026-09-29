// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../phase/phase.hpp"

namespace SOUND::CORE::OSCILLATOR {

using PHASE::Wheel;

constexpr Whole SAW = 0;
constexpr Whole PULSE = 1;
constexpr Whole TRIANGLE = 2;
constexpr Whole WAVES = 3;

constexpr Float SQUARE = 0.5f;
constexpr Float NARROWEST = 0.01f;
constexpr Float WIDEST = 0.99f;

struct Oscillator {
  Wheel phase = 0;
  Wheel step = 0;
  Wheel edge = Wheel(SQUARE * PHASE::TURN);
};

void settle(Oscillator &oscillator, Float hertz, Float width, Whole rate);
void reset(Oscillator &oscillator, Wheel phase);
auto tick(Oscillator &oscillator, Whole wave) -> Float;

auto blep(Wheel phase, Wheel step) -> Float;
auto blamp(Wheel phase, Wheel step) -> Float;

auto saw(const Oscillator &oscillator) -> Float;
auto pulse(const Oscillator &oscillator) -> Float;
auto triangle(const Oscillator &oscillator) -> Float;
auto shaped(const Oscillator &oscillator, Whole wave) -> Float;

namespace NAIVE {
auto saw(Wheel phase) -> Float;
auto pulse(Wheel phase, Wheel edge) -> Float;
auto triangle(Wheel phase) -> Float;
auto shaped(Wheel phase, Wheel edge, Whole wave) -> Float;
auto tick(Oscillator &oscillator, Whole wave) -> Float;
}  // namespace NAIVE

struct Sync {
  Oscillator slave;
  Float pending = 0;
};

auto tick(Sync &sync, const Oscillator &master, Whole wave) -> Float;

}  // namespace SOUND::CORE::OSCILLATOR
