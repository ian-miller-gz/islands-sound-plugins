// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "percussion.hpp"

namespace {

constexpr Float ONE = 1.0f;
constexpr Float BASE = 4.0f;
constexpr Float FLAT = 0;

auto bent(Float drop, Float power) -> Float {
  return power == ONE ? drop : std::pow(drop, power);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Sweep &sweep, Float decay, Float seconds, Float depth, Whole rate) {
  settle(sweep, decay, seconds, depth, FLAT, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Sweep &sweep, Float decay, Float seconds, Float depth, Float slope,
  Whole rate) {
  const Float held = slope < -ONE ? -ONE : slope > ONE ? ONE : slope;
  sweep.body.damp = fall(decay, rate);
  shape(sweep.drop, seconds, rate);
  sweep.depth = depth;
  sweep.power = std::pow(BASE, held);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Sweep &sweep, Float base, Float level) {
  sweep.base = base;
  ENVELOPE::strike(sweep.gate, sweep.drop, ONE);
  strike(sweep.body, level);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Sweep &sweep) -> Float {
  const Float drop = ENVELOPE::tick(sweep.gate, sweep.drop);
  sweep.body.turn =
    sweep.base * (ONE + sweep.depth * ::bent(drop, sweep.power));
  return tick(sweep.body);
}
