// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "shaper.hpp"
#include "../../core/block/block.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::SHAPER {

constexpr Float STALE = -1.0f;

struct Stage {
  CORE::SHAPER::Oversampler oversampler;
  CORE::SHAPER::Oversampler inner;
  CORE::SHAPER::Crusher crusher;
  CORE::SHAPER::Chebyshev chebyshev;
  CORE::SHAPER::Integral hard;
  CORE::SHAPER::Integral fold;
  CORE::SHAPER::Blocker blocker;
  Float decibels = STALE;
  Float gain = 1;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<Stage> stages;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);
auto curved(const Module &module, Stage &stage, Float in) -> Float;
auto shaped(const Module &module, Stage &stage, Float in, Float cv) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SHAPER
