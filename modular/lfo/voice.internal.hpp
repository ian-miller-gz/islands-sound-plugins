// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "lfo.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::LFO {

static_assert(SINE == CORE::MODULATOR::SINE);
static_assert(RANDOM == CORE::MODULATOR::RANDOM);

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::MODULATOR::Lfo lfo;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::LFO
