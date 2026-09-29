// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

using PERCUSSION::RIMS;

constexpr Float RATIOS[RIMS] = {0.182f, 0.667f};
constexpr Float LEVELS[RIMS] = {0.8f, 0.6f};
constexpr Float SHORTER = 0.6f;
constexpr Float EDGE = 300.0f;
constexpr Float CRACK = 3.0f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clave &clave, Float tune, Float decay, Whole rate) {
  settle(clave.bar, tune, decay, rate);
  for (Whole at = 0; at < RIMS; ++at)
    settle(clave.rims[at], tune * RATIOS[at], decay * SHORTER, rate);
  FILTER::settle(clave.edge, FILTER::HIGH, EDGE, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Clave &clave, Whole click, Float velocity) {
  const Float level = struck(velocity);
  if (click != CLICK::RIM) {
    strike(clave.bar, level);
    return;
  }
  for (Whole at = 0; at < RIMS; ++at)
    strike(clave.rims[at], level * LEVELS[at]);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Clave &clave) -> Float {
  Float rim = 0;
  for (Whole at = 0; at < RIMS; ++at) rim += tick(clave.rims[at]);
  const Float crack = SHAPER::soft(FILTER::tick(clave.edge, rim) * CRACK);
  return SHAPER::soft(tick(clave.bar) + crack);
}
