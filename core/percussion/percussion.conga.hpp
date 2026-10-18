// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

constexpr Whole MODES = 2;

struct Head {
  Float tune = 0;
  Float decay = 0;
  Float bend = 0;
  Float drop = 0;
  Float slope = 0;
  Float tone = 0;
  Float ring = 0;
  Float curve = 0;
};

struct Stroke {
  Whole height = HEIGHT::MID;
  Flag muted = false;
};

struct Mode {
  Resonator resonator;
  Float open = 0;
  Float muted = 0;
};

struct Conga {
  Sweep sweep;
  Mode modes[MODES];
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  Float bases[HEIGHT::HEIGHTS] = {};
  Float open = 0;
  Float muted = 0;
  Float bend = 0;
  Float slap = 0;
  Float velocity = 0;
  Float ring = 0;
  Float curve = 0;
  Whole rate = 0;
};

void settle(Conga &conga, Float tune, Float decay, Float bend, Whole rate);
void settle(Conga &conga, const Head &head, Whole rate);
void strike(Conga &conga, Whole height, Float velocity);
void strike(Conga &conga, const Stroke &stroke, Float velocity);
void strike(Conga &conga, Float hertz, Float velocity);
auto tick(Conga &conga) -> Float;
auto sounding(const Conga &conga) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
