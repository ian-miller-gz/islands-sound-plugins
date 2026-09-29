// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "resonator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::RESONATOR {

constexpr Whole STRIDE = 16;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::FILTER::Variable> filters;
  CORE::MODULATOR::Lfo lfo;
  Whole countdown = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void sweep(Module &module, Float swing);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::RESONATOR
