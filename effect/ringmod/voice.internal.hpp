// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "ringmod.hpp"
#include "../../core/block/block.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::RINGMOD {

constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::OSCILLATOR::Oscillator carrier;
  CORE::OSCILLATOR::Table table;
  Float rows[PARAMETERS] = {};
  Whole wave = SINE;
  Float dry = 0;
  Float wet = UNITY;
  CORE::BLOCK::Meter meter;
};

void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);
auto tick(Effect &effect) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::RINGMOD
