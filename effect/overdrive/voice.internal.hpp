// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "overdrive.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::OVERDRIVE {

constexpr Float HUMP = 720.0f;
constexpr Float GENTLEST = 12.0f;
constexpr Float HOTTEST = 118.0f;
constexpr Float DARKEST = 500.0f;
constexpr Float BRIGHTEST = 8000.0f;
constexpr Float UNITY = 1.0f;

struct Strip {
  CORE::FILTER::Pole hump;
  CORE::FILTER::Pole tone;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  Float gain = UNITY;
  Float level = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::OVERDRIVE
