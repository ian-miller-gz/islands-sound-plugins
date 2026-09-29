// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::SHAPER {

using Curve = auto (*)(Float in) -> Float;

auto soft(Float in) -> Float;
auto hard(Float in) -> Float;
auto tube(Float in) -> Float;
auto fold(Float in) -> Float;

struct Crusher {
  Float step = 0;
  Float stride = 1;
  Float phase = 1;
  Float held = 0;
};

void settle(Crusher &crusher, Float bits, Float hertz, Whole rate);
auto tick(Crusher &crusher, Float in) -> Float;

struct Chebyshev {
  Float even = 0;
  Float odd = 0;
};

void settle(Chebyshev &chebyshev, Float even, Float odd);
auto tick(const Chebyshev &chebyshev, Float in) -> Float;

struct Blocker {
  Float pole = 0;
  Float in = 0;
  Float out = 0;
};

void settle(Blocker &blocker, Float cutoff, Whole rate);
auto tick(Blocker &blocker, Float in) -> Float;

constexpr Whole SIDE = 6;
constexpr Whole SPAN = 2 * SIDE;

struct Pair {
  Float early = 0;
  Float late = 0;
};

struct Oversampler {
  Float ups[SPAN] = {};
  Float odds[SPAN] = {};
  Float evens[SIDE] = {};
};

auto up(Oversampler &oversampler, Float in) -> Pair;
auto down(Oversampler &oversampler, Pair pair) -> Float;

template <class Shape>
auto tick(Oversampler &oversampler, Float in, Shape &&shape) -> Float {
  const Pair pair = up(oversampler, in);
  const Pair shaped = {shape(pair.early), shape(pair.late)};
  return down(oversampler, shaped);
}

}  // namespace SOUND::PLUGINS::CORE::SHAPER
