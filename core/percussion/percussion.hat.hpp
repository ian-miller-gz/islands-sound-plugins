// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace OPENING {

enum Opening : Whole { CLOSED, OPEN, OPENINGS };

}  // namespace OPENING

struct Hat {
  Metal metal;
  FILTER::Biquad band;
  FILTER::Biquad high;
  ENVELOPE::Envelope envelopes[OPENING::OPENINGS];
  ENVELOPE::Gate gate;
  Whole opening = OPENING::CLOSED;
  Float velocity = 0;
};

void settle(
  Hat &hat, Float tune, Float closed, Float open, Float tone, Whole rate);
void strike(Hat &hat, Whole opening, Float velocity);
auto tick(Hat &hat) -> Float;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
