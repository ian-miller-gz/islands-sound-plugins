// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "phaser.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PHASER {

constexpr Float OCTAVES = 2.0f;
constexpr Float HALF = 0.5f;
constexpr Float DOUBLE = 2.0f;

struct Channel {
  CORE::FILTER::Pole stages[MOST];
  Float fed = 0;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Channel> strips;
  Float rows[PARAMETERS] = {};
  CORE::MODULATOR::Lfo lfo;
  CORE::FILTER::Pole pole;
  Whole stages = FEWEST;
  Float swing = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PHASER
