// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.hybrid.hpp"

namespace SOUND::CORE::PERCUSSION {

struct Pad {
  HYBRID::Swept tone;
  ENVELOPE::Envelope decay;
  ENVELOPE::Gate gate;
  NOISE::White white;
  Float pace = 0;
  Float top = 0;
  Float floor = 0;
  Float low = 0;
  Float noise = 0;
  Float click = 0;
  Whole width = 0;
  Whole left = 0;
  Float velocity = 0;
};

struct Voicing {
  Float tune = 0;
  Float bend = 0;
  Float decay = 0;
  Float tone = 0;
  Float noise = 0;
  Float click = 0;
};

void settle(Pad &pad, const Voicing &voicing, Whole rate);
void strike(Pad &pad, Float velocity);
auto tick(Pad &pad) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
