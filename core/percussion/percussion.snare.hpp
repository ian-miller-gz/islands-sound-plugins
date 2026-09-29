// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

struct Snare {
  Resonator low;
  Resonator high;
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Pole floor;
  ENVELOPE::Envelope rattle;
  ENVELOPE::Gate gate;
  Float snappy = 0;
  Float velocity = 0;
};

void settle(
  Snare &snare, Float tune, Float tone, Float snappy, Float decay, Whole rate);
void strike(Snare &snare, Float velocity);
auto tick(Snare &snare) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
