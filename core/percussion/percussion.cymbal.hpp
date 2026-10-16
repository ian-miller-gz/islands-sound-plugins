// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.choke.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace PLATE {

enum Plate : Whole { RIDE, CRASH, BELL, PLATES };

}  // namespace PLATE

namespace TAIL {

enum Tail : Whole { PING, WASH, SIZZLE, TAILS };

}  // namespace TAIL

struct Bronze {
  Float tune = 0;
  Float decay = 0;
  Float tone = 0;
  Float splash = 0;
  Float edge = 0;
  Float noise = 0;
  Float curve = 0;
  Float touch = 0;
};

struct Cymbal {
  Metal metal;
  NOISE::White white;
  FILTER::Biquad bands[TAIL::TAILS];
  FILTER::Biquad edge;
  ENVELOPE::Envelope tails[PLATE::PLATES][TAIL::TAILS];
  ENVELOPE::Envelope flash;
  ENVELOPE::Gate gates[TAIL::TAILS];
  ENVELOPE::Gate burst;
  Choke choke;
  Float weights[TAIL::TAILS] = {};
  Float gains[TAIL::TAILS] = {};
  Float airs[TAIL::TAILS] = {};
  Float splash = 0;
  Float flare = 0;
  Float curve = 0;
  Float touch = 0;
  Whole plate = PLATE::CRASH;
  Float velocity = 0;
};

void settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate);
void settle(Cymbal &cymbal, const Bronze &bronze, Whole rate);
void strike(Cymbal &cymbal, Whole plate, Float velocity);
void lift(Cymbal &cymbal);
auto tick(Cymbal &cymbal) -> Float;
auto sounding(const Cymbal &cymbal) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
