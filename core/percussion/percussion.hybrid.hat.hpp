// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.hat.hpp"

namespace SOUND::CORE::PERCUSSION::HYBRID {

struct Hat {
  Vector<Float> loop;
  Float at = 0;
  Float speed = 1;
  ENVELOPE::Envelope envelopes[OPENINGS];
  ENVELOPE::Gate gate;
  Whole opening = CLOSED;
  Float velocity = 0;
};

void build(Hat &hat, Whole rate);
void settle(Hat &hat, Float tune, Float closed, Float open, Whole rate);
void strike(Hat &hat, Whole opening, Float velocity);
auto tick(Hat &hat) -> Float;

}  // namespace SOUND::CORE::PERCUSSION::HYBRID
