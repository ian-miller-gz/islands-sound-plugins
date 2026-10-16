// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::NOISE {

constexpr Float EDGE = 1.0f;

struct Velvet {
  White white;
  PHASE::Wheel phase = 0;
  PHASE::Wheel step = 0;
  PHASE::Wheel spot = 0;
  Float sign = EDGE;
  Flag struck = false;
};

struct Walk {
  White white;
  PHASE::Wheel phase = 0;
  PHASE::Wheel step = 0;
  Float reach = 0;
  Float pole = EDGE;
  Float target = 0;
  Float value = 0;
};

void seed(Velvet &velvet, Register state);
void settle(Velvet &velvet, Float hertz, Whole rate);
auto tick(Velvet &velvet) -> Float;

void seed(Walk &walk, Register state);
void settle(Walk &walk, Float hertz, Float step, Whole rate);
auto tick(Walk &walk) -> Float;

}  // namespace SOUND::PLUGINS::CORE::NOISE
