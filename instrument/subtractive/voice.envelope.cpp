// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SUBTRACTIVE::breathe(
  const Envelope &envelope, Gate &gate) {
  switch (gate.stage) {
    case RISING:
      gate.level += envelope.rise;
      if (gate.level < 1.0f) return;
      gate.level = 1.0f;
      gate.stage = FALLING;
      return;
    case FALLING:
      gate.level -= envelope.fall;
      if (envelope.fall > 0 && gate.level > envelope.sustain) return;
      gate.level = envelope.sustain;
      gate.stage = HELD;
      return;
    case HELD:
      gate.level = envelope.sustain;
      return;
    case LEAVING:
      gate.level -= envelope.drop;
      if (gate.level > 0) return;
      gate.level = 0;
      gate.stage = IDLE;
      return;
    default:
      return;
  }
}
