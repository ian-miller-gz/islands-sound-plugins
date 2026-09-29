// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "../block/block.hpp"
#include "shaper.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float KNEE = 3.0f;
constexpr Float BEND = 27.0f;
constexpr Float SQUARE = 9.0f;
constexpr Float BIAS = 0.25f;
constexpr Float PERIOD = 4.0f;

}  // namespace

auto SOUND::CORE::SHAPER::soft(Float in) -> Float {
  if (in <= -KNEE) return -ONE;
  if (in >= KNEE) return ONE;
  const Float square = in * in;
  return in * (BEND + square) / (BEND + SQUARE * square);
}

auto SOUND::CORE::SHAPER::hard(Float in) -> Float { return BLOCK::clipped(in); }

auto SOUND::CORE::SHAPER::tube(Float in) -> Float {
  return soft(in + BIAS) - soft(BIAS);
}

auto SOUND::CORE::SHAPER::fold(Float in) -> Float {
  const Float shifted = in - ONE;
  const Float wrapped = shifted - PERIOD * std::floor(shifted / PERIOD);
  return std::fabs(wrapped - TWO) - ONE;
}
