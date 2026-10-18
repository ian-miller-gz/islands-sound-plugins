// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../block/block.hpp"

namespace SOUND::PLUGINS::CORE::STEREO {

constexpr Float CENTRE = 60.0f;
constexpr Float SPAN = 30.0f;

struct Pair {
  Float left = 0;
  Float right = 0;
};

auto pan(Float place) -> Pair;
auto place(Float pitch, Float width) -> Float;
void add(Pair &sum, Float in, Pair gains);
auto magnitude(Pair pair) -> Float;
void pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame, Pair pair);

}  // namespace SOUND::PLUGINS::CORE::STEREO
