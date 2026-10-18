// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

struct Choke {
  Float level = 1;
  Float step = 1;
  Flag closed = false;
};

void settle(Choke &choke, Float seconds, Whole rate);
void open(Choke &choke);
void close(Choke &choke);
auto tick(Choke &choke) -> Float;
auto sounding(const Choke &choke) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
