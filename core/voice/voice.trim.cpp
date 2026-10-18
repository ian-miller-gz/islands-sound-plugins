// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

void SOUND::PLUGINS::CORE::VOICE::draw(
  Trim *trims, Whole count, MODULATOR::Seed seed) {
  for (Whole slot = 0; slot < count; ++slot) {
    Trim &trim = trims[slot];
    trim.pitch = MODULATOR::draw(seed);
    trim.cutoff = MODULATOR::draw(seed);
    trim.time = MODULATOR::draw(seed);
  }
}
