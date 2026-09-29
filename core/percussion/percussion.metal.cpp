// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

void SOUND::CORE::PERCUSSION::settle(
  Metal &metal, const Float *hertz, Whole count, Float ratio, Whole rate) {
  metal.count = count < BANK ? count : BANK;
  for (Whole at = 0; at < metal.count; ++at)
    OSCILLATOR::settle(
      metal.oscillators[at], hertz[at] * ratio, OSCILLATOR::SQUARE, rate);
}

auto SOUND::CORE::PERCUSSION::tick(Metal &metal) -> Float {
  if (metal.count == 0) return 0;
  Float sum = 0;
  for (Whole at = 0; at < metal.count; ++at)
    sum += OSCILLATOR::tick(metal.oscillators[at], OSCILLATOR::PULSE);
  return sum / Float(metal.count);
}
