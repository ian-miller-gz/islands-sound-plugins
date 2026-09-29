// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "deesser.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::DEESSER {

constexpr Float KNEE = 3.0f;
constexpr Float MILLI = 1000.0f;

struct Deesser {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::FILTER::Biquad> filters;
  Vector<Float> bands;
  CORE::DYNAMICS::Detector detector;
  CORE::DYNAMICS::Computer computer;
  Whole mode = BAND;
  Flag listen = false;
  CORE::BLOCK::Meter meter;
};

void build(Deesser &deesser);
void settle(Deesser &deesser);
void apply(Deesser &deesser, const AUDIO::PLUGIN::Event &event);
auto reduced(Deesser &deesser, Float band) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::DEESSER
