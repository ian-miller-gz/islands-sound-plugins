// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {

constexpr Float ONE = 1.0f;

}  // namespace

void SOUND::CORE::PERCUSSION::settle(
  Sweep &sweep, Float decay, Float seconds, Float depth, Whole rate) {
  sweep.body.damp = fall(decay, rate);
  shape(sweep.drop, seconds, rate);
  sweep.depth = depth;
}

void SOUND::CORE::PERCUSSION::strike(Sweep &sweep, Float base, Float level) {
  sweep.base = base;
  ENVELOPE::strike(sweep.gate, sweep.drop, ONE);
  strike(sweep.body, level);
}

auto SOUND::CORE::PERCUSSION::tick(Sweep &sweep) -> Float {
  const Float drop = ENVELOPE::tick(sweep.gate, sweep.drop);
  sweep.body.turn = sweep.base * (ONE + sweep.depth * drop);
  return tick(sweep.body);
}
