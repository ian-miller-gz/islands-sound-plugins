// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "grain.hpp"
#include "../../core/block/block.hpp"
#include "../../core/cloud/cloud.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::GRAIN {

constexpr Whole GRAINS = 64;
constexpr Float UNITY = 1.0f;
constexpr Float SEMITONE = 100.0f;
constexpr Float SECOND = 1000.0f;
constexpr Float HALF = 0.5f;

struct Strip {
  CORE::LINE::Line line;
  CORE::CLOUD::Cloud cloud;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  CORE::CLOUD::Grain grain;
  Float lift = 0;
  Float spray = 0;
  Float interval = UNITY;
  Float countdown = 0;
  Float feedback = 0;
  Float dry = 0;
  Float wet = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);
void sow(Effect &effect);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::GRAIN
