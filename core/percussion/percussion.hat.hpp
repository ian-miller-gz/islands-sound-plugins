// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

enum Opening : Whole { CLOSED, OPEN, OPENINGS };

struct Hat {
  Metal metal;
  FILTER::Biquad band;
  FILTER::Biquad high;
  ENVELOPE::Envelope envelopes[OPENINGS];
  ENVELOPE::Gate gate;
  Whole opening = CLOSED;
  Float velocity = 0;
};

void settle(
  Hat &hat, Float tune, Float closed, Float open, Float tone, Whole rate);
void strike(Hat &hat, Whole opening, Float velocity);
auto tick(Hat &hat) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
