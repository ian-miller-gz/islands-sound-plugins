// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "resonator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::RESONATOR {

constexpr Whole STRIDE = 4;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::FILTER::Variable> filters;
  CORE::MODULATOR::Lfo lfo;
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void sweep(Module &module, Whole channel, Float swing, Float cv);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::RESONATOR
