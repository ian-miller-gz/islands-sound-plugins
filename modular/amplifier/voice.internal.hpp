// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "amplifier.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::AMPLIFIER {

constexpr Whole STRIDE = 16;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::FILTER::Variable> filters;
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

struct Tap {
  Whole channel = 0;
  Float control = 0;
  Float sample = 0;
  Flag due = false;
};

void apply(Module &module, const AUDIO::PLUGIN::Event &event);

auto amplify(Module &module, const Tap &tap) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::AMPLIFIER
