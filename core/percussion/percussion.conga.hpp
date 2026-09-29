// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

struct Conga {
  Sweep sweep;
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  Float bases[HEIGHTS] = {};
  Float open = 0;
  Float muted = 0;
  Float bend = 0;
  Float slap = 0;
  Float velocity = 0;
};

void settle(Conga &conga, Float tune, Float decay, Float bend, Whole rate);
void strike(Conga &conga, Whole height, Float velocity);
auto tick(Conga &conga) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
