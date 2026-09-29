// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "shaper.hpp"
#include "../../core/block/block.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::SHAPER {

struct Stage {
  CORE::SHAPER::Oversampler oversampler;
  CORE::SHAPER::Crusher crusher;
  CORE::SHAPER::Blocker blocker;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 1;
  Vector<Stage> stages;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);
auto shaped(const Module &module, Stage &stage, Float in) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SHAPER
