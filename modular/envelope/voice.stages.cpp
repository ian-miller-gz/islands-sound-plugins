// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto counted(Float seconds, Whole rate) -> Whole {
  return Whole(CORE::ENVELOPE::frames(seconds, rate));
}

void begin(ENVELOPE::Module &module) {
  module.left.hold = 0;
  CORE::ENVELOPE::strike(module.gate, module.envelope, module.velocity);
}

void await(ENVELOPE::Module &module) {
  if (module.left.delay == 0) return;
  --module.left.delay;
  if (module.left.delay == 0) begin(module);
}

}  // namespace

auto SOUND::PLUGINS::ENVELOPE::spanned(const Float *rows, Whole rate)
  -> Countdown {
  return {::counted(rows[DELAY], rate), ::counted(rows[HOLD], rate)};
}

void SOUND::PLUGINS::ENVELOPE::strike(Module &module, Float velocity) {
  module.velocity = velocity;
  module.left.delay = module.spans.delay;
  if (module.left.delay == 0) ::begin(module);
}

void SOUND::PLUGINS::ENVELOPE::lift(Module &module) {
  module.left = {};
  CORE::ENVELOPE::lift(module.gate);
}

auto SOUND::PLUGINS::ENVELOPE::step(Module &module) -> Float {
  CORE::ENVELOPE::Gate &gate = module.gate;
  ::await(module);
  if (module.left.hold > 0) {
    --module.left.hold;
    return gate.level * gate.scale;
  }
  const Whole stage = gate.stage;
  const Float level = CORE::ENVELOPE::tick(gate, module.envelope);
  const Flag peaked =
    stage == CORE::ENVELOPE::RISING && gate.stage == CORE::ENVELOPE::FALLING;
  if (peaked) module.left.hold = module.spans.hold;
  return level;
}
