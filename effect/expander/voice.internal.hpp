// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "expander.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace SOUND::PLUGINS::EXPANDER {

constexpr Float MILLI = 0.001f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::DYNAMICS::Detector detector;
  CORE::DYNAMICS::Computer computer;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::EXPANDER
