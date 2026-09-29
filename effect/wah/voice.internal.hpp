// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "wah.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::WAH {

constexpr Float HEEL = 350;
constexpr Float MILLI = 0.001f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::FILTER::Variable> strips;
  CORE::FILTER::Variable shape;
  CORE::DYNAMICS::Detector detector;
  Float gain = UNITY;
  Float span = 0;
  Float trim = UNITY;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void sweep(Effect &effect, Float level);

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::WAH
