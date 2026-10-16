// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "choir.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/reverb/reverb.hpp"
#include "../../core/stereo/stereo.hpp"

namespace SOUND::PLUGINS::CHOIR {

constexpr Whole TAPS = 3;
constexpr Whole SIDES = CORE::REVERB::SIDES;

using Stereo = CORE::STEREO::Pair;

struct Seat {
  Float detune = 0;
  Float shift = 0;
  Float pan = 0;
  Float pace = 0;
};

struct Singer {
  CORE::GLOTTIS::Source source;
  CORE::FILTER::Formant formant;
  CORE::MODULATOR::Lfo vibrato;
  CORE::MODULATOR::Drift wobble;
  CORE::MODULATOR::Drift jitter;
  CORE::ENVELOPE::Gate gate;
  Seat seat;
  Float shift = 1;
  Stereo pan = {1, 1};
  Whole wait = 0;
  Flag pending = false;
};

struct Part {
  Singer singers[SEATS];
};

struct Side {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[TAPS];
};

struct Ensemble {
  Side sides[SIDES];
  CORE::MODULATOR::Lfo lfos[TAPS];
  Float mix = 0;
};

struct Room {
  CORE::REVERB::Network network;
  Float mix = 0;
};

}  // namespace SOUND::PLUGINS::CHOIR
