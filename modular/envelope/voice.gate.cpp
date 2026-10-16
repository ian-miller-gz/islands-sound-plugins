// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole FIRST = 0;

void still(ENVELOPE::Module &) {}

void rose(ENVELOPE::Module &module) {
  ENVELOPE::strike(module, CORE::ENVELOPE::FULL);
}

void fell(ENVELOPE::Module &module) {
  if (!module.allocator.notes[ENVELOPE::LEAD].held) ENVELOPE::lift(module);
}

using Cross = void (*)(ENVELOPE::Module &);

constexpr Cross CROSSED[CORE::BLOCK::CROSSINGS] = {still, rose, fell};

}  // namespace

void SOUND::PLUGINS::ENVELOPE::follow(
  Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Whole channels = module.channels;
  const Float gate =
    CORE::BLOCK::lane(lanes, IN::GATE, ::FIRST, channels)[frame];
  const Whole crossing = CORE::BLOCK::crossed(module.edge, gate);
  if (crossing < CORE::BLOCK::CROSSINGS) ::CROSSED[crossing](module);
  if (!CORE::BLOCK::due(module.stride, STRIDE)) return;
  const Float cv = CORE::BLOCK::lane(lanes, IN::CV, ::FIRST, channels)[frame];
  if (cv == module.cv) return;
  module.cv = cv;
  settle(module);
}
