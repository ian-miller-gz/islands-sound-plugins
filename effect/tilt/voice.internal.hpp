// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "tilt.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::TILT {

constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::FILTER::Pole> poles;
  Float rows[PARAMETERS] = {};
  Float lows = UNITY;
  Float highs = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::TILT
