// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {
void synced();
void levelled();
}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole TICKS = 3;
constexpr Float RISE = 1;
constexpr Float FRACTIONS[] = {0, 0.5f, 1};
constexpr Float FALL = -1;
constexpr Float EARLY = 0.25f;
constexpr Float LATE = 0.75f;

void step(Float fraction) {
  CORE::OSCILLATOR::Edges edges;
  CORE::OSCILLATOR::jump(edges, RISE, fraction);
  std::printf("step");
  for (Whole at = 0; at < TICKS; ++at)
    std::printf(
      " %.9g", static_cast<double>(CORE::OSCILLATOR::tick(edges, RISE)));
  std::printf("\n");
}

void blip() {
  CORE::OSCILLATOR::Edges edges;
  CORE::OSCILLATOR::jump(edges, RISE, EARLY);
  CORE::OSCILLATOR::jump(edges, FALL, LATE);
  Float sum = 0;
  std::printf("blip");
  for (Whole at = 0; at < TICKS; ++at) {
    const Float out = CORE::OSCILLATOR::tick(edges, 0);
    sum += out;
    std::printf(" %.9g", static_cast<double>(out));
  }
  std::printf("\nsum %.9g\n", static_cast<double>(sum));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {

void run() {
  for (const Float fraction : ::FRACTIONS) ::step(fraction);
  ::blip();
  synced();
  levelled();
}

}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR
