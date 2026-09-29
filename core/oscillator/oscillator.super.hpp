// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "oscillator.wave.hpp"

namespace SOUND::PLUGINS::CORE::OSCILLATOR {

constexpr Whole SAWS = 7;
constexpr Whole MIDDLE = SAWS / 2;

struct Super {
  Oscillator saws[SAWS];
  Float centre = 1;
  Float side = 0;
};

auto spread(Float detune) -> Float;
auto centre(Float mix) -> Float;
auto side(Float mix) -> Float;

void settle(Super &super, Float hertz, Float detune, Float mix, Whole rate);
void scatter(Super &super, Wheel seed);
auto tick(Super &super) -> Float;

}  // namespace SOUND::PLUGINS::CORE::OSCILLATOR
