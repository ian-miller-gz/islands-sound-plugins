// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float HALF = 0.5f;

}  // namespace

auto SOUND::PLUGINS::CORE::OSCILLATOR::cubic(
  const Float *points, Float fraction) -> Float {
  const Float before = points[0];
  const Float here = points[1];
  const Float there = points[2];
  const Float after = points[3];
  const Float slope = HALF * (there - before);
  const Float bend = before - (TWO + HALF) * here + TWO * there - HALF * after;
  const Float twist = HALF * (after - before) + (ONE + HALF) * (here - there);
  return ((twist * fraction + bend) * fraction + slope) * fraction + here;
}
