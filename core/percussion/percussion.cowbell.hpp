// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

struct Cowbell {
  Metal metal;
  FILTER::Biquad band;
  ENVELOPE::Envelope clank;
  ENVELOPE::Envelope tail;
  ENVELOPE::Gate hit;
  ENVELOPE::Gate ring;
  Float velocity = 0;
};

void settle(Cowbell &cowbell, Float tune, Float decay, Whole rate);
void strike(Cowbell &cowbell, Float velocity);
auto tick(Cowbell &cowbell) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
