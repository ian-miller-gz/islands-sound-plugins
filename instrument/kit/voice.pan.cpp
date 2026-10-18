// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::KIT::panned(Whole index) -> Whole {
  return index >= PANS && index < CURVE ? index - PANS : DRUMS;
}

void SOUND::PLUGINS::KIT::pan(Kit &kit, Whole drum) {
  if (drum >= DRUMS) return;
  kit.sides[drum] = CORE::STEREO::pan(kit.rows[PANS + drum]);
}
