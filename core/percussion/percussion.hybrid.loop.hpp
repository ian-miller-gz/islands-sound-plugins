// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID {

struct Colour {
  Float band = 0;
  Float edge = 0;
};

struct Loop {
  Vector<Float> samples;
  Float at = 0;
  Float speed = 1;
};

void build(Loop &loop, Float seconds, const Colour &colour, Whole rate);
void settle(Loop &loop, Float tune);
void strike(Loop &loop);
auto tick(Loop &loop) -> Float;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID
