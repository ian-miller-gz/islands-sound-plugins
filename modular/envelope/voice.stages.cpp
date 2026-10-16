// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::ENVELOPE::strike(Module &module, Float velocity) {
  CORE::ENVELOPE::strike(module.gate, module.envelope, velocity);
}

void SOUND::PLUGINS::ENVELOPE::lift(Module &module) {
  CORE::ENVELOPE::lift(module.gate);
}
