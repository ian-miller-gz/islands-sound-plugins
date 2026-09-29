// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "widener.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::WIDENER {

constexpr Whole ORDER = 2;
constexpr Whole SIDES = 2;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Strip {
  CORE::FILTER::Biquad highs[ORDER];
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float mid = UNITY;
  Float side = UNITY;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::WIDENER
