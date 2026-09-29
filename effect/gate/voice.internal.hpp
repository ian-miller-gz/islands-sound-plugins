// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "gate.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace SOUND::PLUGINS::GATE {

constexpr Float MILLI = 0.001f;
constexpr Float SENSE = 0.01f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::DYNAMICS::Highpass> keys;
  CORE::DYNAMICS::Detector sense;
  CORE::DYNAMICS::Hold hold;
  CORE::DYNAMICS::Detector envelope;
  Float rows[PARAMETERS] = {};
  Float threshold = 0;
  Float floor = 0;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::GATE
