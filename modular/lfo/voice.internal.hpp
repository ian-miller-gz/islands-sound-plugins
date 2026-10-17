// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "lfo.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::LFO {

static_assert(SINE == CORE::MODULATOR::SINE);
static_assert(RANDOM == CORE::MODULATOR::RANDOM);

constexpr Float FACTOR = 10.0f;
constexpr Whole STRIDE = 16;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float cv = 0;
  CORE::MODULATOR::Lfo lfo;
  CORE::BLOCK::Edge edge;
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void tune(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);
void follow(Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::LFO
