// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../modulator/modulator.hpp"
#include "voice.hpp"

namespace SOUND::PLUGINS::CORE::VOICE {

struct Trim {
  Float pitch = 0;
  Float cutoff = 0;
  Float time = 0;
};

void draw(Trim *trims, Whole count, MODULATOR::Seed seed);

template <Whole COUNT>
void draw(Trim (&trims)[COUNT], MODULATOR::Seed seed) {
  draw(trims, COUNT, seed);
}

}  // namespace SOUND::PLUGINS::CORE::VOICE
