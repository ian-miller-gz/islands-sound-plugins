// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "parametric.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::PARAMETRIC {

inline constexpr Whole KINDS[BANDS] = {
  CORE::FILTER::LOWSHELF, CORE::FILTER::PEAK, CORE::FILTER::PEAK,
  CORE::FILTER::HIGHSHELF};

struct Strip {
  CORE::FILTER::Biquad bands[BANDS];
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

auto tick(Strip &strip, Float in) -> Float;

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::PARAMETRIC
