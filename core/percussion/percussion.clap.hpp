// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

constexpr Whole BURSTS = 4;

struct Clap {
  NOISE::White white;
  FILTER::Biquad band;
  ENVELOPE::Envelope tail;
  ENVELOPE::Gate gate;
  Whole gap = 0;
  Whole clock = 0;
  Whole left = 0;
  Float burst = 0;
  Float fall = 0;
  Float velocity = 0;
};

void settle(Clap &clap, Float spread, Float decay, Float tone, Whole rate);
void strike(Clap &clap, Float velocity);
auto tick(Clap &clap) -> Float;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
