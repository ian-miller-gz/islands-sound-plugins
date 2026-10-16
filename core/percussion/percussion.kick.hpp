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
  Float bend = 0;
  Float drop = 0;
  Float slope = 0;
  Float sigh = 0;
  Float curve = 0;
  Float touch = 0;
};

struct Kick {
  Sweep body;
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate bend;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  FILTER::Pole hush;
  FILTER::Pole tone;
  Float base = 0;
  Float hertz = 0;
  Float top = 0;
  Float attack = 0;
  Float drive = 1;
  Float sigh = 0;
  Float breath = 0;
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
void strike(Kick &kick, Float hertz, Float velocity);
auto tick(Kick &kick) -> Float;
void darken(Kick &kick);
auto sounding(const Kick &kick) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
