// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole FIRST = 0;

}  // namespace

void SOUND::PLUGINS::NOISE::follow(
  Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float cv =
    CORE::BLOCK::lane(lanes, CV, ::FIRST, module.channels)[frame];
  if (!CORE::BLOCK::due(module.stride, STRIDE)) return;
  module.cv = cv;
  tune(module);
}
