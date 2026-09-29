// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "exciter.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::EXCITER {

constexpr Float UNITY = 1.0f;
constexpr Float DECIBELS = 20.0f;
constexpr Float TEN = 10.0f;

struct Strip {
  CORE::FILTER::Biquad split;
  CORE::FILTER::Biquad clean;
  CORE::SHAPER::Oversampler oversampler;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  Float drive = UNITY;
  Float trim = UNITY;
  Float mix = 0;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::EXCITER
