// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole FIRST = 0;

}  // namespace

void SOUND::PLUGINS::LFO::follow(
  Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Whole channels = module.channels;
  const Float sync = CORE::BLOCK::lane(lanes, SYNC, ::FIRST, channels)[frame];
  const Float cv = CORE::BLOCK::lane(lanes, CV, ::FIRST, channels)[frame];
  if (CORE::BLOCK::crossed(module.edge, sync) == CORE::BLOCK::ROSE)
    CORE::MODULATOR::reset(module.lfo);
  if (!CORE::BLOCK::due(module.stride, STRIDE)) return;
  module.cv = cv;
  tune(module);
}
