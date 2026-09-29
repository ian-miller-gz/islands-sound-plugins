// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace PLATE {

enum Plate : Whole { RIDE, CRASH, PLATES };

}  // namespace PLATE

namespace TAIL {

enum Tail : Whole { PING, WASH, TAILS };

}  // namespace TAIL

struct Cymbal {
  Metal metal;
  FILTER::Biquad bands[TAIL::TAILS];
  ENVELOPE::Envelope tails[PLATE::PLATES][TAIL::TAILS];
  ENVELOPE::Envelope flash;
  ENVELOPE::Gate gates[TAIL::TAILS];
  ENVELOPE::Gate burst;
  Float weights[TAIL::TAILS] = {};
  Float splash = 0;
  Whole plate = PLATE::CRASH;
  Float velocity = 0;
};

void settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate);
void strike(Cymbal &cymbal, Whole plate, Float velocity);
auto tick(Cymbal &cymbal) -> Float;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
