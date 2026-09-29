// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "freeze.hpp"
#include "../../core/block/block.hpp"
#include "../../core/cloud/cloud.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::FREEZE {

constexpr Whole GRAINS = 4;
constexpr Float OVERLAP = 2.0f;
constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float SECOND = 1000.0f;
constexpr Float SILENCE = 0.001f;
constexpr Float QUIET = 0.0001f;
constexpr Float SMOOTHING = 0.01f;

struct Strip {
  CORE::LINE::Line line;
  CORE::CLOUD::Cloud cloud;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  CORE::CLOUD::Grain grain;
  CORE::MODULATOR::Smoother latch;
  Flag held = false;
  Float interval = UNITY;
  Float countdown = 0;
  Float fade = UNITY;
  Float level = UNITY;
  Float mix = 0;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void engage(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);
void sow(Effect &effect);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::FREEZE
