// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::CORE::PERCUSSION {

enum Click : Whole { CLAVE, RIM, CLICKS };

constexpr Whole RIMS = 2;

struct Clave {
  Resonator bar;
  Resonator rims[RIMS];
  FILTER::Pole edge;
};

void settle(Clave &clave, Float tune, Float decay, Whole rate);
void strike(Clave &clave, Whole click, Float velocity);
auto tick(Clave &clave) -> Float;

}  // namespace SOUND::CORE::PERCUSSION
