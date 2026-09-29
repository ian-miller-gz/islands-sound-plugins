// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "multiband.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::MULTIBAND {

constexpr Whole ORDER = 2;
constexpr Whole LOW = 0;
constexpr Whole MID = 1;
constexpr Whole HIGH = 2;
constexpr Whole FIRST = 0;
constexpr Whole LAST = CROSSOVERS - 1;
constexpr Float KNEE = 6;
constexpr Float MILLI = 0.001f;

struct Split {
  CORE::FILTER::Biquad lows[ORDER];
  CORE::FILTER::Biquad highs[ORDER];
};

struct Strip {
  Split splits[CROSSOVERS];
  Split allpass;
  Float parts[BANDS] = {};
};

struct Band {
  CORE::DYNAMICS::Detector detector;
  CORE::DYNAMICS::Computer computer;
  Float makeup = CORE::DYNAMICS::UNITY;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Band bands[BANDS];
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void divide(Strip &strip, Float in);

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::MULTIBAND
