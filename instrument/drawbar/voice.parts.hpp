// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "drawbar.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::DRAWBAR {

struct Pair {
  Float left = 0;
  Float right = 0;
};

struct Scanner {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweep;
  CORE::MODULATOR::Lfo lfo;
  Float wet = 0;
  Float dry = 1;
};

struct Rotor {
  CORE::LINE::Line line;
  CORE::LINE::Sweep near;
  CORE::LINE::Sweep far;
  CORE::MODULATOR::Smoother speed;
  CORE::PHASE::Wheel phase = 0;
  Float target = 0;
  Float swing = 0;
};

struct Rotary {
  CORE::FILTER::Biquad low;
  CORE::FILTER::Biquad high;
  Rotor horn;
  Rotor drum;
  Flag spinning = false;
};

struct Contacts {
  Whole waits[CORE::PHASE::PITCHES][BARS] = {};
  Whole pending = 0;
  Float spread = 0;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
};

}  // namespace SOUND::PLUGINS::DRAWBAR
