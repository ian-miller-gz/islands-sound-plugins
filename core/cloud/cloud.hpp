// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../line/line.hpp"

namespace SOUND::PLUGINS::CORE::CLOUD {

constexpr Whole POINTS = 1024;

struct Grain {
  Float delay = 0;
  Float step = 0;
  Float span = 0;
  Float gain = 0;
  Float age = 0;
  Flag live = false;
};

struct Cloud {
  Vector<Grain> grains;
  Vector<Float> window;
};

void build(Cloud &cloud, Whole count);
void clear(Cloud &cloud);
auto start(Cloud &cloud, const Grain &grain) -> Flag;
auto tick(Cloud &cloud, const LINE::Line &line) -> Float;

}  // namespace SOUND::PLUGINS::CORE::CLOUD
