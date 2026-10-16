// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "resonator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::RESONATOR {

constexpr Whole STRIDE = 4;
constexpr Float BOOST = 7.0f;
constexpr Whole TWICE = 2;

struct Stage {
  CORE::FILTER::Variable variable;
  CORE::FILTER::Ladder ladder;
  CORE::FILTER::Diode diode;
  CORE::FILTER::Sallen sallen;
  CORE::SHAPER::Oversampler oversampler;
};

struct Tone {
  Whole kind = 0;
  Float cutoff = 0;
  Float emphasis = 0;
  Float drive = 0;
  Whole rate = 0;
};

struct Model {
  void (*settle)(Stage &stage, const Tone &tone);
  auto (*tick)(Stage &stage, Float in) -> Float;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<Stage> stages;
  Whole model = VARIABLE;
  CORE::MODULATOR::Lfo lfo;
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

auto chosen(const Module &module) -> Whole;
auto model(const Module &module) -> const Model &;
void settle(Module &module);
void sweep(Module &module, Whole channel, Float swing, Float cv);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::RESONATOR
