// SPDX-License-Identifier: AGPL-3.0-or-later
#include "shaper.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float TWO = 2.0f;
constexpr Float HALF = 0.5f;

constexpr Float TAPS[] = {0.309390819f,  -0.082054191f, 0.030557533f,
                          -0.010060781f, 0.002349427f,  -0.000182808f};
static_assert(sizeof(TAPS) / sizeof(TAPS[0]) == SHAPER::SIDE);

template <Whole COUNT>
void push(Float (&history)[COUNT], Float value) {
  for (Whole at = COUNT - 1; at > 0; --at) history[at] = history[at - 1];
  history[0] = value;
}

auto folded(const Float (&history)[SHAPER::SPAN]) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < SHAPER::SIDE; ++at)
    sum +=
      TAPS[at] * (history[SHAPER::SIDE - 1 - at] + history[SHAPER::SIDE + at]);
  return sum;
}

}  // namespace

auto SOUND::CORE::SHAPER::up(Oversampler &oversampler, Float in) -> Pair {
  ::push(oversampler.ups, in);
  return {TWO * ::folded(oversampler.ups), oversampler.ups[SIDE - 1]};
}

auto SOUND::CORE::SHAPER::down(Oversampler &oversampler, Pair pair) -> Float {
  ::push(oversampler.odds, pair.late);
  ::push(oversampler.evens, pair.early);
  return ::folded(oversampler.odds) + HALF * oversampler.evens[SIDE - 1];
}
