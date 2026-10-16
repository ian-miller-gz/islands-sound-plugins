// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

constexpr Whole BURSTS = 4;

struct Hand {
  Float spread = 0;
  Float decay = 0;
  Float tone = 0;
  Float tail = 0;
  Float cutoff = 0;
  Float emphasis = 0;
  Whole bursts = BURSTS;
  Float scatter = 0;
  Float curve = 0;
  Float touch = 0;
};

struct Clap {
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Biquad ring;
  FILTER::Variable low;
  ENVELOPE::Envelope tail;
  ENVELOPE::Gate gate;
  Hand hand;
  Flag open = true;
  Whole gap = 0;
  Whole next = 0;
  Whole clock = 0;
  Whole left = 0;
  Float span = 0;
  Float burst = 0;
  Float fall = 0;
  Float velocity = 0;
};

void settle(Clap &clap, Float spread, Float decay, Float tone, Whole rate);
void settle(Clap &clap, const Hand &hand, Whole rate);
void strike(Clap &clap, Float velocity);
auto tick(Clap &clap) -> Float;
auto sounding(const Clap &clap) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
