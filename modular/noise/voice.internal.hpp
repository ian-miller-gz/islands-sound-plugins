// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "noise.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"

namespace SOUND::PLUGINS::NOISE {

constexpr Float SMOOTH = 0.005f;
constexpr CORE::NOISE::Register SPACING = 0x9E3779B9u;

struct Source {
  CORE::NOISE::White white;
  CORE::NOISE::Pink pink;
  CORE::NOISE::Brown brown;
  CORE::NOISE::Blue blue;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<Source> sources;
  CORE::MODULATOR::Smoother level;
  Float target = 0;
  CORE::BLOCK::Meter meter;
};

void seed(Module &module);
void settle(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::NOISE
