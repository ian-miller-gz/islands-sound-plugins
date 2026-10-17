// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace CLICK {

enum Click : Whole { CLAVE, RIM, CLICKS };

}  // namespace CLICK

namespace CLAVE {

constexpr Whole RIMS = 2;
constexpr Whole MODES = 2;

}  // namespace CLAVE

struct Clave {
  Resonator bar;
  Resonator modes[CLAVE::MODES];
  Resonator rims[CLAVE::RIMS];
  FILTER::Pole edge;
};

void settle(Clave &clave, Float tune, Float decay, Whole rate);
void settle(
  Clave &clave, Float tune, Float decay, Float shift, Float edge, Whole rate);
void strike(Clave &clave, Whole click, Float velocity);
void strike(Clave &clave, Whole click, Float level, Float ring);
auto tick(Clave &clave) -> Float;
auto sounding(const Clave &clave) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
