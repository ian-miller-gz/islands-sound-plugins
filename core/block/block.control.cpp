// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "block.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole SINGLE = 1;

auto held(Float value, Float least, Float most) -> Float {
  return value > least ? (value < most ? value : most) : least;
}

}  // namespace

auto SOUND::PLUGINS::CORE::BLOCK::due(Stride &stride, Whole every) -> Flag {
  if (stride.left > 0) {
    --stride.left;
    return false;
  }
  stride.left = (every == 0 ? SINGLE : every) - SINGLE;
  return true;
}

void SOUND::PLUGINS::CORE::BLOCK::reset(Stride &stride) { stride.left = 0; }

auto SOUND::PLUGINS::CORE::BLOCK::lane(
  AUDIO::PLUGIN::Sample *const *lanes, Whole in, Whole channel,
  Whole channels) -> AUDIO::PLUGIN::Sample * {
  return lanes[in * channels + channel];
}

auto SOUND::PLUGINS::CORE::BLOCK::crossed(Edge &edge, Float in) -> Whole {
  const Float last = edge.last;
  edge.last = in;
  if (!edge.high && in > UPPER) {
    edge.high = true;
    edge.since = (in - UPPER) / (in - last);
    return ROSE;
  }
  if (edge.high && in < LOWER) {
    edge.high = false;
    return FELL;
  }
  return STILL;
}

auto SOUND::PLUGINS::CORE::BLOCK::scaled(
  Float base, Float cv, Float octaves, Float least, Float most) -> Float {
  return ::held(base * std::exp2(octaves * cv), least, most);
}
