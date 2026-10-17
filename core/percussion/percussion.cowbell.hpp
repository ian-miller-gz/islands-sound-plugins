// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

struct Cowbell {
  Metal metal;
  FILTER::Biquad band;
  ENVELOPE::Envelope clank;
  ENVELOPE::Envelope tail;
  ENVELOPE::Gate hit;
  ENVELOPE::Gate ring;
  Float velocity = 0;
  Float bite = 0;
  Float body = 0;
};

void settle(Cowbell &cowbell, Float tune, Float decay, Whole rate);
void settle(
  Cowbell &cowbell, Float tune, Float decay, Float interval, Float tone,
  Whole rate);
void strike(Cowbell &cowbell, Float velocity);
void strike(Cowbell &cowbell, Float level, Float clank, Float ring);
auto tick(Cowbell &cowbell) -> Float;
auto sounding(const Cowbell &cowbell) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
