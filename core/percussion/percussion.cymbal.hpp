// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

enum Plate : Whole { RIDE, CRASH, PLATES };

enum Tail : Whole { PING, WASH, TAILS };

struct Cymbal {
  Metal metal;
  FILTER::Biquad bands[TAILS];
  ENVELOPE::Envelope tails[PLATES][TAILS];
  ENVELOPE::Envelope flash;
  ENVELOPE::Gate gates[TAILS];
  ENVELOPE::Gate burst;
  Float weights[TAILS] = {};
  Float splash = 0;
  Whole plate = CRASH;
  Float velocity = 0;
};

void settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate);
void strike(Cymbal &cymbal, Whole plate, Float velocity);
auto tick(Cymbal &cymbal) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
