// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "graphic.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::GRAPHIC {

constexpr Float OCTAVE = 1.41421356f;

inline constexpr Float CENTRES[BANDS] = {31.5f, 63,   125,  250,  500,
                                         1000,  2000, 4000, 8000, 16000};

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

}  // namespace SOUND::GRAPHIC
