// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.hat.hpp"
#include "percussion.hybrid.loop.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID {

struct Hat {
  Loop loop;
  ENVELOPE::Envelope envelopes[OPENING::OPENINGS];
  ENVELOPE::Gate gate;
  Whole opening = OPENING::CLOSED;
  Float velocity = 0;
};

void build(Hat &hat, Whole rate);
void settle(Hat &hat, Float tune, Float closed, Float open, Whole rate);
void strike(Hat &hat, Whole opening, Float velocity);
auto tick(Hat &hat) -> Float;
auto sounding(const Hat &hat) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID
