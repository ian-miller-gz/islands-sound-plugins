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

constexpr Flag ENDED[CORE::ENVELOPE::STAGES] = {
  true, false, false, true, false};

auto never(const ENVELOPE::Module &) -> Flag { return false; }

auto pressed(const ENVELOPE::Module &module) -> Flag {
  return module.edge.high || module.allocator.notes[ENVELOPE::LEAD].held;
}

auto always(const ENVELOPE::Module &) -> Flag { return true; }

using Rule = auto (*)(const ENVELOPE::Module &) -> Flag;

constexpr Rule LOOPED[] = {never, pressed, always};
static_assert(std::size(LOOPED) == std::size(ENVELOPE::LOOPS));

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

void SOUND::PLUGINS::ENVELOPE::loop(Module &module) {
  const Whole stage = module.gate.stage;
  if (stage >= CORE::ENVELOPE::STAGES || !::ENDED[stage]) return;
  if (module.left.delay > 0 || module.left.hold > 0) return;
  const Whole mode = Whole(module.rows[LOOP]);
  if (mode < std::size(::LOOPED) && ::LOOPED[mode](module))
    strike(module, module.velocity);
}
