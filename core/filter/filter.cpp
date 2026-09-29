// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float PI = 3.14159265f;

struct Mix {
  Float direct;
  Float low;
};

constexpr Mix MIXES[] = {{0, 1}, {1, -1}};
constexpr Whole SIDES = sizeof(MIXES) / sizeof(MIXES[0]);

}  // namespace

auto SOUND::PLUGINS::CORE::FILTER::bounded(Float cutoff, Whole rate) -> Float {
  const Float top = Float(rate) * CEILING;
  return cutoff < LOWEST ? LOWEST : cutoff > top ? top : cutoff;
}

auto SOUND::PLUGINS::CORE::FILTER::bounded(Float emphasis) -> Float {
  return emphasis < 0 ? 0 : emphasis > ONE ? ONE : emphasis;
}

auto SOUND::PLUGINS::CORE::FILTER::warped(Float cutoff, Whole rate) -> Float {
  return std::tan(PI * bounded(cutoff, rate) / Float(rate));
}

void SOUND::PLUGINS::CORE::FILTER::settle(
  Pole &pole, Whole kind, Float cutoff, Whole rate) {
  const Float g = warped(cutoff, rate);
  const Mix &mix = MIXES[kind < SIDES ? kind : LOW];
  pole.gain = g / (ONE + g);
  pole.direct = mix.direct;
  pole.low = mix.low;
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Pole &pole, Float in) -> Float {
  const Float step = (in - pole.state) * pole.gain;
  const Float low = step + pole.state;
  pole.state = low + step;
  return pole.direct * in + pole.low * low;
}
