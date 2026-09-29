// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION::HYBRID {

struct Bend {
  Float depth = 0;
  Float fast = 0;
  Float knee = 0;
  Float slow = 0;
};

struct Swept {
  PHASE::Wheel phase = 0;
  Float pace = 0;
  Float depth = 0;
  Float knee = 0;
  ENVELOPE::Envelope fast;
  ENVELOPE::Envelope slow;
  ENVELOPE::Gate head;
  ENVELOPE::Gate tail;
};

auto pace(Float hertz, Whole rate) -> Float;
auto rounded(PHASE::Wheel phase) -> Float;

void settle(Swept &swept, const Bend &bend, Whole rate);
void strike(Swept &swept, Float pace);
auto tick(Swept &swept) -> Float;

struct Kick {
  Swept body;
  ENVELOPE::Envelope decay;
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate gate;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  Float pace = 0;
  Float attack = 0;
  Whole width = 0;
  Whole left = 0;
  Float velocity = 0;
};

void settle(Kick &kick, Float tune, Float decay, Float attack, Whole rate);
void strike(Kick &kick, Float velocity);
auto tick(Kick &kick) -> Float;

struct Snare {
  Swept low;
  Swept high;
  ENVELOPE::Envelope body;
  ENVELOPE::Envelope rattle;
  ENVELOPE::Gate tone;
  ENVELOPE::Gate snap;
  NOISE::White white;
  FILTER::Pole dark;
  FILTER::Pole floor;
  Float lower = 0;
  Float upper = 0;
  Float snappy = 0;
  Float velocity = 0;
};

void settle(
  Snare &snare, Float tune, Float decay, Float snappy, Float tone, Whole rate);
void strike(Snare &snare, Float velocity);
auto tick(Snare &snare) -> Float;

struct Tom {
  Swept body;
  ENVELOPE::Envelope decay;
  ENVELOPE::Envelope skin;
  ENVELOPE::Gate gate;
  ENVELOPE::Gate hit;
  NOISE::White white;
  FILTER::Pole tone;
  Float paces[HEIGHTS] = {};
  Float velocity = 0;
};

void settle(Tom &tom, Float tune, Float decay, Whole rate);
void strike(Tom &tom, Whole height, Float velocity);
auto tick(Tom &tom) -> Float;

}  // namespace SOUND::CORE::PERCUSSION::HYBRID

#include "percussion.hybrid.hat.hpp"
