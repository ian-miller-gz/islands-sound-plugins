// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float RATIOS[PERCUSSION::HEIGHT::HEIGHTS] = {1.0f, 1.47f, 2.16f};
constexpr Float BEND = 0.5f;
constexpr Float SWEEP = 0.03f;
constexpr Float MUTE = 0.25f;
constexpr Float SNAP = 0.008f;
constexpr Float CRISP = 1500.0f;
constexpr Float SOFT = 0.75f;
constexpr Float SLAP = 0.5f;
constexpr Float ONE = 1.0f;

auto slapped(Float velocity) -> Float {
  const Float over = (velocity - SOFT) / (ONE - SOFT);
  return over < 0 ? 0 : over;
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Conga &conga, Float tune, Float decay, Float bend, Whole rate) {
  for (Whole height = 0; height < HEIGHT::HEIGHTS; ++height)
    conga.bases[height] = turn(tune * RATIOS[height], rate);
  conga.bend = bend * BEND;
  settle(conga.sweep, decay, SWEEP, conga.bend, rate);
  conga.open = conga.sweep.body.damp;
  conga.muted = fall(decay * MUTE, rate);
  shape(conga.snap, SNAP, rate);
  FILTER::settle(conga.crisp, FILTER::HIGH, CRISP, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Conga &conga, Whole height, Float velocity) {
  conga.velocity = struck(velocity);
  conga.slap = ::slapped(conga.velocity);
  const Whole at = height < HEIGHT::HEIGHTS ? height : HEIGHT::MID;
  conga.sweep.depth = conga.bend * (ONE + conga.slap);
  strike(conga.sweep, conga.bases[at], conga.velocity);
  conga.sweep.body.damp = conga.open + (conga.muted - conga.open) * conga.slap;
  ENVELOPE::strike(conga.click, conga.snap, conga.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Conga &conga) -> Float {
  const Float body = tick(conga.sweep);
  const Float level = ENVELOPE::tick(conga.click, conga.snap);
  const Float noise = FILTER::tick(conga.crisp, NOISE::tick(conga.white));
  const Float slap = noise * level * conga.slap * SLAP * conga.velocity;
  return SHAPER::soft(body + slap);
}
