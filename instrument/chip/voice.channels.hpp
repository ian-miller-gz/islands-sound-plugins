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

struct Timer {
  Float debt = 0;
  Float clocks = 0;
};

struct Pulse {
  Whole position = 0;
  Whole duty = 0;
  Whole period = 0;
  Float swept = 1;
  Whole divider = 0;
  Flag muted = true;
};

struct Wave {
  Whole position = 0;
  Flag muted = true;
};

struct Lfsr {
  Whole bits = 1;
  Whole tap = 1;
};

}  // namespace SOUND::PLUGINS::CHIP

namespace SOUND::PLUGINS::CHIP::CHANNEL {

enum Index : Whole { FIRST, SECOND, WAVE, HISS, COUNT };

constexpr Whole SQUARE = 8;
constexpr Whole RAMP = 32;

}  // namespace SOUND::PLUGINS::CHIP::CHANNEL
