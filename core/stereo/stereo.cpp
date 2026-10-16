// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "stereo.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;
constexpr Float ROOT = std::numbers::sqrt2_v<Float>;
constexpr Float QUARTER = std::numbers::pi_v<Float> * 0.25f;
constexpr Whole SIDES = 2;

}  // namespace

auto SOUND::PLUGINS::CORE::STEREO::pan(Float place) -> Pair {
  const Float angle = ::QUARTER * (BLOCK::clipped(place) + ::FULL);
  return {::ROOT * std::cos(angle), ::ROOT * std::sin(angle)};
}

auto SOUND::PLUGINS::CORE::STEREO::place(Float pitch, Float width) -> Float {
  return BLOCK::clipped((pitch - CENTRE) / SPAN * width);
}

void SOUND::PLUGINS::CORE::STEREO::add(Pair &sum, Float in, Pair gains) {
  sum.left += in * gains.left;
  sum.right += in * gains.right;
}

auto SOUND::PLUGINS::CORE::STEREO::magnitude(Pair pair) -> Float {
  const Float left = BLOCK::magnitude(pair.left);
  const Float right = BLOCK::magnitude(pair.right);
  return left > right ? left : right;
}

void SOUND::PLUGINS::CORE::STEREO::pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame, Pair pair) {
  const Float sides[::SIDES] = {pair.left, pair.right};
  const Float mean = (pair.left + pair.right) * ::HALF;
  const Whole sided = channels < ::SIDES ? 0 : ::SIDES;
  for (Whole channel = 0; channel < channels; ++channel)
    lanes[channel][frame] = channel < sided ? sides[channel] : mean;
}
