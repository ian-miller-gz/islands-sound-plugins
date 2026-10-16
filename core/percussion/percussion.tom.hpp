// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.conga.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

struct Tom {
  Sweep sweep;
  Resonator modes[MODES];
  ENVELOPE::Envelope skin;
  ENVELOPE::Gate hit;
  NOISE::White white;
  FILTER::Pole tone;
  Float bases[HEIGHT::HEIGHTS] = {};
  Float velocity = 0;
  Float ring = 0;
  Float curve = 0;
  Float touch = 0;
  Float colour = 0;
  Whole rate = 0;
};

void settle(
  Tom &tom, Float tune, Float decay, Float bend, Float tone, Whole rate);
void settle(Tom &tom, const Head &head, Float touch, Whole rate);
void strike(Tom &tom, Whole height, Float velocity);
void strike(Tom &tom, Float hertz, Float velocity);
auto tick(Tom &tom) -> Float;
auto sounding(const Tom &tom) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
