// SPDX-License-Identifier: AGPL-3.0-or-later
#include "stereo.hpp"

namespace {

constexpr Float HALF = 0.5f;
constexpr Whole SIDES = 2;

}  // namespace

void SOUND::PLUGINS::CORE::STEREO::pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame, Pair pair) {
  const Float sides[SIDES] = {pair.left, pair.right};
  const Float mean = (pair.left + pair.right) * HALF;
  const Whole sided = channels < SIDES ? 0 : SIDES;
  for (Whole channel = 0; channel < channels; ++channel)
    lanes[channel][frame] = channel < sided ? sides[channel] : mean;
}
