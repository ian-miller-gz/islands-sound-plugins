// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "crusher.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::CRUSHER {

constexpr Float SWAY = 0.5f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::SHAPER::Crusher> crushers;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
  Float rows[PARAMETERS] = {};
  Float stride = UNITY;
  Float sway = 0;
  Float dry = 0;
  Float wet = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::CRUSHER
