// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "../../core/percussion/percussion.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole LANES = 3;
constexpr Whole FRAMES = 4;
constexpr Whole LAST = FRAMES - 1;
constexpr CORE::STEREO::Pair SIDED = {0.5f, -0.25f};
constexpr CORE::STEREO::Pair LOUD = {3.0f, 0};
constexpr Float SAMPLE = -0.5f;
constexpr Whole ALONE = 1;
constexpr Whole BOTH = 2;
constexpr Float OVER = 2.0f;

struct Lanes {
  AUDIO::PLUGIN::Sample samples[LANES][FRAMES] = {};
  AUDIO::PLUGIN::Sample *lanes[LANES] = {};
};

template <class Value>
void play(STRING::Hot word, Whole channels, Value value) {
  Lanes lanes;
  for (Whole at = 0; at < LANES; ++at) lanes.lanes[at] = lanes.samples[at];
  const auto sound = [value] { return value; };
  const auto apply = [](const AUDIO::PLUGIN::Event &) {};
  const Float peak = CORE::PERCUSSION::play(
    lanes.lanes, channels, FRAMES, nullptr, 0, sound, apply);
  std::printf("%s %.9g", word, double(peak));
  for (Whole at = 0; at < channels; ++at)
    std::printf(" %.9g", double(lanes.samples[at][LAST]));
  std::printf("\n");
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::PERCUSSION {

void play() {
  ::play("single", ALONE, SIDED);
  ::play("double", BOTH, SIDED);
  ::play("triple", LANES, SIDED);
  ::play("clip", BOTH, LOUD);
  ::play("mono", BOTH, SAMPLE);
  ::play("over", BOTH, OVER);
}

}  // namespace SOUND::PLUGINS::HARNESS::PERCUSSION
