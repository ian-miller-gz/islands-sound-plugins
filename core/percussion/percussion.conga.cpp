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
constexpr Float SLAP = 0.5f;
constexpr Float FLAT = 0;
constexpr Float ONE = 1.0f;
constexpr Float TOP = 1.9f;
constexpr Float DIPOLE = 1.59f;
constexpr Float QUADRUPOLE = 2.14f;
constexpr Float OVERTONES[PERCUSSION::MODES] = {DIPOLE, QUADRUPOLE};

void ring(PERCUSSION::Conga &conga, Float decay, Whole rate) {
  for (Whole at = 0; at < PERCUSSION::MODES; ++at) {
    PERCUSSION::Mode &mode = conga.modes[at];
    mode.open = PERCUSSION::fall(decay / OVERTONES[at], rate);
    mode.muted = PERCUSSION::fall(decay * MUTE / OVERTONES[at], rate);
  }
}

auto sing(PERCUSSION::Conga &conga) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < PERCUSSION::MODES; ++at) {
    PERCUSSION::Resonator &mode = conga.modes[at].resonator;
    const Float turn = conga.sweep.body.turn * OVERTONES[at];
    mode.turn = turn < TOP ? turn : TOP;
    sum += PERCUSSION::tick(mode);
  }
  return sum / Float(PERCUSSION::MODES);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Conga &conga, Float tune, Float decay, Float bend, Whole rate) {
  settle(
    conga,
    Head{
      .tune = tune,
      .decay = decay,
      .bend = bend,
      .drop = SWEEP,
      .slope = FLAT,
      .tone = CRISP},
    rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Conga &conga, const Head &head, Whole rate) {
  for (Whole height = 0; height < HEIGHT::HEIGHTS; ++height)
    conga.bases[height] = turn(head.tune * RATIOS[height], rate);
  conga.bend = head.bend * BEND;
  conga.curve = head.curve;
  conga.rate = rate;
  settle(conga.sweep, head.decay, head.drop, conga.bend, head.slope, rate);
  conga.open = conga.sweep.body.damp;
  conga.muted = fall(head.decay * MUTE, rate);
  ::ring(conga, head.decay, rate);
  conga.ring = head.ring < 0 ? 0 : head.ring > ONE ? ONE : head.ring;
  shape(conga.snap, SNAP, rate);
  FILTER::settle(conga.crisp, FILTER::HIGH, head.tone, rate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Conga &conga) -> Float {
  if (!sounding(conga)) return 0;
  const Float body = tick(conga.sweep);
  const Float upper = ::sing(conga);
  const Float tone = body + (upper - body) * conga.ring;
  const Float level = ENVELOPE::tick(conga.click, conga.snap);
  const Float noise = FILTER::tick(conga.crisp, NOISE::tick(conga.white));
  const Float slap = noise * level * conga.slap * SLAP * conga.velocity;
  return SHAPER::soft(tone + slap);
}
