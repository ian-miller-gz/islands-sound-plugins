// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.choke.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace OPENING {

enum Opening : Whole { CLOSED, OPEN, PEDAL, OPENINGS };

}  // namespace OPENING

namespace METAL {

enum Kind : Whole { SQUARES, RINGS, KINDS };

}  // namespace METAL

struct Setting {
  Float pedal = 0;
  Float noise = 0;
  Whole metal = METAL::SQUARES;
  Float touch = 0;
};

namespace HAT {

struct Stroke {
  ENVELOPE::Gate gate;
  Float velocity = 0;
};

}  // namespace HAT

struct Hat {
  Metal metal;
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Biquad high;
  ENVELOPE::Envelope envelopes[OPENING::OPENINGS];
  HAT::Stroke shut;
  HAT::Stroke open;
  Choke choke;
  Setting setting;
  Whole opening = OPENING::CLOSED;
  Whole rate = 0;
  Float shade = 1;
};

void settle(
  Hat &hat, Float tune, Float closed, Float open, Float tone, Whole rate);
void settle(Hat &hat, const Setting &setting, Whole rate);
void strike(Hat &hat, Whole opening, Float velocity);
auto ring(Metal &metal) -> Float;
auto tick(Hat &hat) -> Float;
auto sounding(const Hat &hat) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
