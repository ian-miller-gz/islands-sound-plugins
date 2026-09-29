// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../block/block.hpp"
#include "shaper.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float THREE = 3.0f;
constexpr Float FOUR = 4.0f;
constexpr Float TURN = 6.28318531f;

}  // namespace

void SOUND::CORE::SHAPER::settle(Chebyshev &chebyshev, Float even, Float odd) {
  chebyshev.even = even;
  chebyshev.odd = odd;
}

auto SOUND::CORE::SHAPER::tick(const Chebyshev &chebyshev, Float in) -> Float {
  const Float held = BLOCK::clipped(in);
  const Float square = held * held;
  const Float third = held * (FOUR * square - THREE);
  return in + chebyshev.even * square + chebyshev.odd * third;
}

void SOUND::CORE::SHAPER::settle(Blocker &blocker, Float cutoff, Whole rate) {
  const Float pole = ONE - TURN * cutoff / Float(rate);
  blocker.pole = pole < 0 ? 0 : pole > ONE ? ONE : pole;
}

auto SOUND::CORE::SHAPER::tick(Blocker &blocker, Float in) -> Float {
  blocker.out = in - blocker.in + blocker.pole * blocker.out;
  blocker.in = in;
  return blocker.out;
}
