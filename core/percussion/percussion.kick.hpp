// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

struct Batter {
  Float tune = 0;
  Float decay = 0;
  Float attack = 0;
  Float drive = 0;
  Float tone = 0;
  Float curve = 0;
  Float touch = 0;
};

struct Kick {
  Resonator body;
  ENVELOPE::Envelope sweep;
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate bend;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  FILTER::Pole tone;
  Float base = 0;
  Float attack = 0;
  Float drive = 1;
  Float colour = 0;
  Float curve = 0;
  Float touch = 0;
  Float velocity = 0;
  Whole rate = 0;
};

void settle(
  Kick &kick, Float tune, Float decay, Float attack, Float drive, Float tone,
  Whole rate);
void settle(Kick &kick, const Batter &batter, Whole rate);
void strike(Kick &kick, Float velocity);
auto tick(Kick &kick) -> Float;
auto sounding(const Kick &kick) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
