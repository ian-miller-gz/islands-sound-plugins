// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "distortion.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::DISTORTION {

constexpr Float TIGHT = 80.0f;
constexpr Float STILL = 10.0f;
constexpr Float GENTLEST = 2.0f;
constexpr Float HOTTEST = 1000.0f;
constexpr Float DARKEST = 475.0f;
constexpr Float BRIGHTEST = 12000.0f;
constexpr Float UNITY = 1.0f;

struct Strip {
  CORE::FILTER::Pole tight;
  CORE::SHAPER::Oversampler oversampler;
  CORE::SHAPER::Blocker blocker;
  CORE::FILTER::Pole tone;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  CORE::SHAPER::Curve curve = CORE::SHAPER::hard;
  Float gain = UNITY;
  Float level = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::DISTORTION
