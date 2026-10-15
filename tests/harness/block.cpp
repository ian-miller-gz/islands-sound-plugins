// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "../../core/block/block.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole EVERY = 16;
constexpr Whole CALLS = 64;
constexpr Whole MOVED = 5;
constexpr Whole CHANNELS = 2;
constexpr Whole INS = 3;
constexpr Whole LANES = INS * CHANNELS;
constexpr Whole IN = 2;
constexpr Whole CHANNEL = 1;
constexpr Float WAVE[] = {0, 0.08f, 0.2f, 0.07f, 0.04f, 0.09f, 0.5f};

struct Case {
  Float base;
  Float cv;
  Float octaves;
};

constexpr Float LEAST = 20;
constexpr Float MOST = 20000;
constexpr Case CASES[] = {
  {100, 1, 1}, {100, -1, 2}, {100, 10, 1}, {100, -10, 1}};

void stride(STRING::Hot word, Whole moved) {
  CORE::BLOCK::Stride stride;
  std::printf("%s", word);
  for (Whole call = 0; call < CALLS; ++call) {
    if (call == moved) CORE::BLOCK::reset(stride);
    if (CORE::BLOCK::due(stride, EVERY)) std::printf(" %zu", call);
  }
  std::printf("\n");
}

void lane() {
  AUDIO::PLUGIN::Sample samples[LANES] = {};
  AUDIO::PLUGIN::Sample *lanes[LANES];
  for (Whole at = 0; at < LANES; ++at) lanes[at] = &samples[at];
  const AUDIO::PLUGIN::Sample *found =
    CORE::BLOCK::lane(lanes, IN, CHANNEL, CHANNELS);
  std::printf("lane %td\n", found - samples);
}

void edge() {
  CORE::BLOCK::Edge edge;
  std::printf("edge");
  for (const Float in : WAVE)
    std::printf(" %zu", CORE::BLOCK::crossed(edge, in));
  std::printf("\n");
  edge = {};
  std::printf("since");
  for (const Float in : WAVE)
    if (CORE::BLOCK::crossed(edge, in) == CORE::BLOCK::ROSE)
      std::printf(" %.9g", static_cast<double>(edge.since));
  std::printf("\n");
}

void scaled() {
  std::printf("scaled");
  for (const Case &at : CASES) {
    const Float value =
      CORE::BLOCK::scaled(at.base, at.cv, at.octaves, LEAST, MOST);
    std::printf(" %.9g", static_cast<double>(value));
  }
  std::printf("\n");
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::BLOCK {

void run() {
  ::stride("due", CALLS);
  ::stride("reset", MOVED);
  ::lane();
  ::edge();
  ::scaled();
}

}  // namespace SOUND::PLUGINS::HARNESS::BLOCK
