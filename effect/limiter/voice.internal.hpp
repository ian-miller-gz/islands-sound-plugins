// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "limiter.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace SOUND::LIMITER {

constexpr Float AHEAD = 0.005f;
constexpr Float RAMP = 0.2f;
constexpr Float MILLI = 0.001f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::DYNAMICS::Ring> rings;
  CORE::DYNAMICS::Hold hold;
  CORE::DYNAMICS::Detector detector;
  Float rows[PARAMETERS] = {};
  Float ceiling = UNITY;
  Float gain = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::LIMITER
