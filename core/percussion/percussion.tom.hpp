// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

struct Tom {
  Sweep sweep;
  ENVELOPE::Envelope skin;
  ENVELOPE::Gate hit;
  NOISE::White white;
  FILTER::Pole tone;
  Float bases[HEIGHTS] = {};
  Float velocity = 0;
};

void settle(
  Tom &tom, Float tune, Float decay, Float bend, Float tone, Whole rate);
void strike(Tom &tom, Whole height, Float velocity);
auto tick(Tom &tom) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
