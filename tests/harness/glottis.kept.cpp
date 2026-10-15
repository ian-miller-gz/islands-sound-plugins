// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cstdint>
#include <cstdio>

#include "../../core/glottis/glottis.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Whole RATE = 48000;
constexpr Float HERTZ = 110;
constexpr Float OPEN = 0.6f;
constexpr Float BREATH = 0.2f;
constexpr Float CLOSED = 0.25f;
constexpr Float FULL = 1;

struct Today {
  Float place = CORE::GLOTTIS::place(OPEN);
  CORE::PHASE::Wheel edge = CORE::PHASE::Wheel(OPEN * CORE::PHASE::TURN);
  CORE::PHASE::Wheel step = CORE::PHASE::step(HERTZ, RATE);
  CORE::PHASE::Wheel phase = 0;
  CORE::NOISE::White white;
};

auto tick(Today &today, const CORE::OSCILLATOR::Table &table) -> Float {
  const Float pulse = CORE::OSCILLATOR::morph(table, today.place, today.phase);
  const Float gate = today.phase < today.edge ? FULL : CLOSED;
  const Float noise = CORE::NOISE::tick(today.white) * gate;
  today.phase += today.step;
  return pulse + (noise - pulse) * BREATH;
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::GLOTTIS {

void kept(const CORE::OSCILLATOR::Table &table) {
  CORE::GLOTTIS::Source source;
  CORE::GLOTTIS::settle(source, FULL, FULL, RATE);
  CORE::GLOTTIS::settle(source, OPEN, BREATH);
  CORE::GLOTTIS::tune(source, HERTZ, RATE);
  Today today;
  Whole differing = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Float out = CORE::GLOTTIS::tick(source, table);
    const Float was = ::tick(today, table);
    differing += std::bit_cast<Bits>(out) != std::bit_cast<Bits>(was);
  }
  std::printf("kept %zu\n", differing);
}

}  // namespace SOUND::PLUGINS::HARNESS::GLOTTIS
