// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr Float HALF = 0.5f;

auto placed(Float fraction) -> Float {
  return fraction < 0 ? 0 : fraction > UNIT ? UNIT : fraction;
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::jump(
  Edges &edges, Float height, Float fraction) {
  const Float before = ::placed(fraction);
  const Float after = UNIT - before;
  edges.now += height * after * after * ::HALF;
  edges.next -= height * before * before * ::HALF;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::tick(Edges &edges, Float level)
  -> Float {
  const Float out = edges.level + edges.now;
  edges.level = level;
  edges.now = edges.next;
  edges.next = 0;
  return out;
}
