// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "mixer.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/stereo/stereo.hpp"

namespace SOUND::PLUGINS::MIXER {

constexpr Whole STEREO = 2;
constexpr Float GLIDE = 0.005f;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float weights[INS][STEREO] = {};
  CORE::MODULATOR::Smoother smoothers[INS][STEREO];
  CORE::BLOCK::Meter meter;
};

void settle(Module &module);
void prime(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::MIXER
