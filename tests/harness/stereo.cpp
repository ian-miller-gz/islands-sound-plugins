// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "../../core/stereo/stereo.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENTRED = 0;
constexpr Float LEFT = -1.0f;
constexpr Float RIGHT = 1.0f;
constexpr Float BEYOND = -3.0f;
constexpr Whole STEPS = 40;
constexpr Float STEP = (RIGHT - LEFT) / Float(STEPS);
constexpr Whole LANES = 4;
constexpr Whole FRAMES = 2;
constexpr Whole FRAME = 1;
constexpr Whole ALONE = 1;
constexpr Whole BOTH = 2;
constexpr CORE::STEREO::Pair SIDED = {0.5f, -0.25f};
constexpr CORE::STEREO::Pair HEARD = {-0.5f, 0.3f};
constexpr Float IN = 0.5f;

struct Case {
  Float pitch;
  Float width;
};

constexpr Case CASES[] = {{60, 1}, {90, 1}, {45, 1}, {120, 1}, {90, 0.5f}};

void print(CORE::STEREO::Pair pair) {
  std::printf(" %.9g %.9g", double(pair.left), double(pair.right));
}

void pan(STRING::Hot word, Float place) {
  std::printf("%s", word);
  ::print(CORE::STEREO::pan(place));
  std::printf("\n");
}

void power() {
  std::printf("power");
  for (Whole step = 0; step <= STEPS; ++step) {
    const CORE::STEREO::Pair gains =
      CORE::STEREO::pan(LEFT + STEP * Float(step));
    const Float sum = gains.left * gains.left + gains.right * gains.right;
    std::printf(" %.9g", double(sum));
  }
  std::printf("\n");
}

void place() {
  std::printf("place");
  for (const Case &at : CASES)
    std::printf(" %.9g", double(CORE::STEREO::place(at.pitch, at.width)));
  std::printf("\n");
}

void pour(STRING::Hot word, Whole channels) {
  AUDIO::PLUGIN::Sample samples[LANES][FRAMES] = {};
  AUDIO::PLUGIN::Sample *lanes[LANES];
  for (Whole at = 0; at < LANES; ++at) lanes[at] = samples[at];
  CORE::STEREO::pour(lanes, channels, FRAME, SIDED);
  std::printf("%s", word);
  for (Whole at = 0; at < channels; ++at)
    std::printf(" %.9g", double(samples[at][FRAME]));
  std::printf("\n");
}

void add() {
  CORE::STEREO::Pair sum = SIDED;
  CORE::STEREO::add(sum, IN, CORE::STEREO::pan(RIGHT));
  std::printf("add");
  ::print(sum);
  std::printf("\n");
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::STEREO {

void run() {
  ::pan("centre", CENTRED);
  ::pan("left", LEFT);
  ::pan("right", RIGHT);
  ::pan("beyond", BEYOND);
  ::power();
  ::place();
  ::pour("single", ALONE);
  ::pour("double", BOTH);
  ::pour("quad", LANES);
  ::add();
  std::printf("magnitude %.9g\n", double(CORE::STEREO::magnitude(HEARD)));
}

}  // namespace SOUND::PLUGINS::HARNESS::STEREO
