// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>

#include "../../core/shaper/shaper.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float WIDTH = 0.001f;
constexpr Float HALF = 0.5f;
constexpr Float POINTS[] = {-2.5f, -0.5f, 0.25f, 0.7f, 1.5f, 2.2f, 3.4f};
constexpr Float SPAN = 3.0f;
constexpr Float PERIOD = 4.0f;
constexpr Float CLOSE = 0.00001f;
constexpr Float START = 0.5f;
constexpr Float LOUD = 50.0f;
constexpr Float SWEEP = 2.3f;
constexpr Whole TICKS = 4096;

using Curve = CORE::SHAPER::Curve;

void slope(STRING::Hot word, Curve curve, Curve area) {
  std::printf("%s", word);
  for (const Float at : POINTS) {
    const Float rise = area(at + WIDTH) - area(at - WIDTH);
    const Float error = rise / (WIDTH + WIDTH) - curve(at);
    std::printf(" %.9g", static_cast<double>(error));
  }
  std::printf("\n");
}

void mean(STRING::Hot word, Curve curve, Curve area, Float to) {
  CORE::SHAPER::Integral integral;
  const Float out = CORE::SHAPER::tick(integral, to, curve, area);
  std::printf("%s %.9g\n", word, static_cast<double>(out));
}

void near(Curve curve, Curve area) {
  CORE::SHAPER::Integral integral{START, area(START)};
  const Float to = START + CLOSE;
  const Float out = CORE::SHAPER::tick(integral, to, curve, area);
  const Float error = out - curve(HALF * (START + to));
  std::printf("near %.9g\n", static_cast<double>(error));
}

void bounded(STRING::Hot word, Curve curve, Curve area) {
  CORE::SHAPER::Integral integral;
  Float largest = 0;
  for (Whole at = 0; at < TICKS; ++at) {
    const Float in = LOUD * std::sin(SWEEP * Float(at));
    const Float out = std::fabs(CORE::SHAPER::tick(integral, in, curve, area));
    if (out > largest) largest = out;
  }
  std::printf("%s %.9g\n", word, static_cast<double>(largest));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::SHAPER {

void run() {
  ::slope("hard", CORE::SHAPER::hard, CORE::SHAPER::AREA::hard);
  ::slope("fold", CORE::SHAPER::fold, CORE::SHAPER::AREA::fold);
  std::printf(
    "origin %.9g %.9g\n", static_cast<double>(CORE::SHAPER::AREA::hard(0)),
    static_cast<double>(CORE::SHAPER::AREA::fold(0)));
  ::mean("clip", CORE::SHAPER::hard, CORE::SHAPER::AREA::hard, ::SPAN);
  ::mean("period", CORE::SHAPER::fold, CORE::SHAPER::AREA::fold, ::PERIOD);
  ::near(CORE::SHAPER::fold, CORE::SHAPER::AREA::fold);
  ::bounded("clipped", CORE::SHAPER::hard, CORE::SHAPER::AREA::hard);
  ::bounded("folded", CORE::SHAPER::fold, CORE::SHAPER::AREA::fold);
}

}  // namespace SOUND::PLUGINS::HARNESS::SHAPER
