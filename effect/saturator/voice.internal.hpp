// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "saturator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::SATURATOR {

constexpr Float STILL = 10.0f;
constexpr Float TRIM = 0.5f;
constexpr Float FLOOR = 0.01f;
constexpr Float UNITY = 1.0f;

struct Strip {
  CORE::SHAPER::Blocker blocker;
  CORE::NOISE::Pink hiss;
  CORE::FILTER::Biquad low;
  CORE::FILTER::Biquad high;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  Float gain = UNITY;
  Float bias = 0;
  Float rest = 0;
  Float trim = UNITY;
  Float hiss = 0;
  Float dry = 0;
  Float wet = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SATURATOR
