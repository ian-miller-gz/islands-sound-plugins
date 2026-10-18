// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

using PERCUSSION::CLAVE::MODES;
using PERCUSSION::CLAVE::RIMS;

constexpr Float RATIOS[RIMS] = {0.182f, 0.667f};
constexpr Float LEVELS[RIMS] = {0.8f, 0.6f};
constexpr Float OVERTONES[MODES] = {2.756f, 5.404f};
constexpr Float SHORTER = 0.6f;
constexpr Float EDGE = 300.0f;
constexpr Float CRACK = 3.0f;
constexpr Float UNSHIFTED = 1.0f;
constexpr Float CEILING = 0.5f;

void overtone(
  PERCUSSION::Resonator &mode, Float hertz, Float seconds, Whole rate) {
  if (hertz < Float(rate) * CEILING) {
    PERCUSSION::settle(mode, hertz, seconds, rate);
    return;
  }
  mode.turn = 0;
  mode.damp = 0;
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clave &clave, Float tune, Float decay, Whole rate) {
  settle(clave, tune, decay, UNSHIFTED, EDGE, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clave &clave, Float tune, Float decay, Float shift, Float edge, Whole rate) {
  settle(clave.bar, tune, decay, rate);
  for (Whole at = 0; at < MODES; ++at)
    ::overtone(
      clave.modes[at], tune * OVERTONES[at], decay / OVERTONES[at], rate);
  for (Whole at = 0; at < RIMS; ++at)
    settle(clave.rims[at], tune * RATIOS[at] * shift, decay * SHORTER, rate);
  FILTER::settle(clave.edge, FILTER::HIGH, edge, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Clave &clave, Whole click, Float velocity) {
  strike(clave, click, struck(velocity), 0);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Clave &clave, Whole click, Float level, Float ring) {
  if (click != CLICK::RIM) {
    strike(clave.bar, level);
    for (Resonator &mode : clave.modes) strike(mode, level * ring);
    return;
  }
  for (Whole at = 0; at < RIMS; ++at)
    strike(clave.rims[at], level * LEVELS[at]);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Clave &clave) -> Float {
  if (!sounding(clave)) return 0;
  Float rim = 0;
  for (Whole at = 0; at < RIMS; ++at) rim += tick(clave.rims[at]);
  const Float crack = SHAPER::soft(FILTER::tick(clave.edge, rim) * CRACK);
  Float bar = tick(clave.bar);
  for (Resonator &mode : clave.modes) bar += tick(mode);
  return SHAPER::soft(bar + crack);
}
