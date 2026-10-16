// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <utility>

#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole PAIR = 2;

void reorder(Float *reals, Float *imags, Whole size) {
  Whole swap = 0;
  for (Whole at = 1; at < size; ++at) {
    Whole bit = size >> 1;
    for (; (swap & bit) != 0; bit >>= 1) swap ^= bit;
    swap ^= bit;
    if (at >= swap) continue;
    std::swap(reals[at], reals[swap]);
    std::swap(imags[at], imags[swap]);
  }
}

void turn(Float *reals, Float *imags, Whole size) {
  for (Whole width = PAIR; width <= size; width <<= 1) {
    const Whole half = width >> 1;
    for (Whole at = 0; at < half; ++at) {
      const Float angle = OSCILLATOR::TAU * Float(at) / Float(width);
      const Float cosine = std::cos(angle);
      const Float sine = std::sin(angle);
      for (Whole low = at; low < size; low += width) {
        const Whole high = low + half;
        const Float real = reals[high] * cosine - imags[high] * sine;
        const Float imag = reals[high] * sine + imags[high] * cosine;
        reals[high] = reals[low] - real;
        imags[high] = imags[low] - imag;
        reals[low] += real;
        imags[low] += imag;
      }
    }
  }
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::transform(
  Float *reals, Float *imags, Whole size) {
  ::reorder(reals, imags, size);
  ::turn(reals, imags, size);
}
