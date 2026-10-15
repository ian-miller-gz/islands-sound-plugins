// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chip.hpp"
#include "../../core/phase/phase.hpp"

namespace SOUND::PLUGINS::CHIP {

struct Clock {
  CORE::PHASE::Wheel phase = 0;
  CORE::PHASE::Wheel step = 0;
};

struct Shape {
  Whole level = 0;
  Whole decay = 0;
  Whole sustain = 0;
};

struct Steps {
  Whole level = 0;
  Whole count = 0;
  Flag held = false;
};

struct Pulse {
  Clock clock;
  Whole duty = 0;
  Flag muted = true;
};

struct Wave {
  Clock clock;
  Flag muted = true;
};

struct Lfsr {
  Whole bits = 1;
  Whole tap = 1;
  Float clocks = 0;
  Float debt = 0;
};

}  // namespace SOUND::PLUGINS::CHIP
