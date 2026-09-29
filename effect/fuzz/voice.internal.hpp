// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "fuzz.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::FUZZ {

constexpr Float FLOOR = 0.00316f;
constexpr Float OPENING = 0.001f;
constexpr Float CLOSING = 0.05f;
constexpr Float STILL = 10.0f;
constexpr Float GENTLEST = 1.0f;
constexpr Float HOTTEST = 30.0f;
constexpr Float LOWS = 408.0f;
constexpr Float HIGHS = 1160.0f;
constexpr Float UNITY = 1.0f;

struct Strip {
  CORE::DYNAMICS::Detector gate;
  CORE::SHAPER::Oversampler oversampler;
  CORE::SHAPER::Blocker blocker;
  CORE::FILTER::Pole low;
  CORE::FILTER::Pole high;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  Float gain = UNITY;
  Float bias = 0;
  Float rest = 0;
  Float highs = 0;
  Float lows = UNITY;
  Float level = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::FUZZ
