// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "breath.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::BREATH {

constexpr Float LIKENESS = 0.5f;
constexpr Float SENSE = 0.005f;
constexpr Float FADE = 0.05f;
constexpr Float MILLI = 1000.0f;

struct Breath {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::FILTER::Biquad> filters;
  Vector<Float> bands;
  CORE::DYNAMICS::Detector whole;
  CORE::DYNAMICS::Detector band;
  CORE::DYNAMICS::Detector gate;
  Float threshold = 0;
  Float floor = 0;
  CORE::BLOCK::Meter meter;
};

void build(Breath &breath);
void settle(Breath &breath);
void apply(Breath &breath, const AUDIO::PLUGIN::Event &event);
auto gated(Breath &breath, Float in, Float high) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::BREATH
