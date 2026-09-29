// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "spring.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/reverb/reverb.hpp"

namespace SOUND::SPRING {

constexpr Whole MONO = 1;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Whole STAGES = 40;
constexpr Float STEP = 1.0f;
constexpr Float LONGEST = 0.060f;
constexpr Float SHORTEST = 0.025f;
constexpr Float LOOSE = 0.75f;
constexpr Float TAUT = 0.55f;
constexpr Float BRIGHT = 4500.0f;
constexpr Float CUT = 120.0f;
constexpr Float LENGTHS[CORE::REVERB::SIDES] = {1.0f, 1.23f};
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Spring {
  CORE::LINE::Line line;
  CORE::LINE::Loop loop;
  CORE::REVERB::Allpass stages[STAGES];
  Float delay = 0;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::FILTER::Pole cut;
  Spring springs[CORE::REVERB::SIDES];
  Float rows[PARAMETERS] = {};
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

void build(Spring &spring, Float seconds, Whole rate);
void settle(
  Spring &spring, Float seconds, Float chirp, Float decay, Whole rate);
auto tick(Spring &spring, Float in) -> Float;

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::SPRING
