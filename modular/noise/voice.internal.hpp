// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "noise.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"

namespace SOUND::PLUGINS::NOISE {

constexpr Float SMOOTH = 0.005f;
constexpr Float REACH = 0.2f;
constexpr Whole STRIDE = 16;
constexpr CORE::NOISE::Register SPACING = 0x9E3779B9u;

struct Source {
  CORE::NOISE::White white;
  CORE::NOISE::Pink pink;
  CORE::NOISE::Brown brown;
  CORE::NOISE::Blue blue;
  CORE::NOISE::Violet violet;
  CORE::NOISE::Velvet velvet;
  CORE::NOISE::Hold hold;
  CORE::NOISE::Walk walk;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float cv = 0;
  Vector<Source> sources;
  CORE::MODULATOR::Smoother level;
  Float target = 0;
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

using Colour = auto (*)(Source &source) -> Float;
using Mode = auto (*)(Source &source, Colour colour) -> Float;

void seed(Module &module);
void settle(Module &module);
void tune(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);
void follow(Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame);
auto mode(const Module &module) -> Mode;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::NOISE
