// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

struct Kick {
  Resonator body;
  ENVELOPE::Envelope sweep;
  ENVELOPE::Envelope snap;
  ENVELOPE::Gate bend;
  ENVELOPE::Gate click;
  NOISE::White white;
  FILTER::Pole crisp;
  FILTER::Pole tone;
  Float base = 0;
  Float attack = 0;
  Float drive = 1;
  Float velocity = 0;
};

void settle(
  Kick &kick, Float tune, Float decay, Float attack, Float drive, Float tone,
  Whole rate);
void strike(Kick &kick, Float velocity);
auto tick(Kick &kick) -> Float;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
