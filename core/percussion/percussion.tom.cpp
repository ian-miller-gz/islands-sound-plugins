// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float RATIOS[PERCUSSION::HEIGHT::HEIGHTS] = {1.0f, 1.5f, 2.0f};
constexpr Float BEND = 1.0f;
constexpr Float SWEEP = 0.5f;
constexpr Float SKIN = 0.02f;
constexpr Float HIT = 0.4f;
constexpr Float FLAT = 0;
constexpr Float ONE = 1.0f;
constexpr Float TOP = 1.9f;
constexpr Float DIPOLE = 1.59f;
constexpr Float QUADRUPOLE = 2.14f;
constexpr Float OVERTONES[PERCUSSION::MODES] = {DIPOLE, QUADRUPOLE};

auto held(Float value) -> Float {
  return value < 0 ? 0 : value > ONE ? ONE : value;
}

auto sing(PERCUSSION::Tom &tom) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < PERCUSSION::MODES; ++at) {
    const Float turn = tom.sweep.body.turn * OVERTONES[at];
    tom.modes[at].turn = turn < TOP ? turn : TOP;
    sum += PERCUSSION::tick(tom.modes[at]);
  }
  return sum / Float(PERCUSSION::MODES);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Tom &tom, Float tune, Float decay, Float bend, Float tone, Whole rate) {
  const Head head = {
    .tune = tune,
    .decay = decay,
    .bend = bend,
    .drop = decay * SWEEP,
    .slope = FLAT,
    .tone = tone};
  settle(tom, head, FLAT, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Tom &tom, const Head &head, Float touch, Whole rate) {
  for (Whole height = 0; height < HEIGHT::HEIGHTS; ++height)
    tom.bases[height] = turn(head.tune * RATIOS[height], rate);
  settle(tom.sweep, head.decay, head.drop, head.bend * BEND, head.slope, rate);
  for (Whole at = 0; at < MODES; ++at)
    tom.modes[at].damp = fall(head.decay / OVERTONES[at], rate);
  shape(tom.skin, SKIN, rate);
  tom.ring = ::held(head.ring);
  tom.curve = head.curve;
  tom.touch = touch;
  tom.colour = head.tone;
  tom.rate = rate;
  FILTER::settle(tom.tone, FILTER::LOW, head.tone, rate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Tom &tom) -> Float {
  if (!sounding(tom)) return 0;
  const Float body = tick(tom.sweep);
  const Float shell = body + (::sing(tom) - body) * tom.ring;
  const Float level = ENVELOPE::tick(tom.hit, tom.skin);
  const Float noise = FILTER::tick(tom.tone, NOISE::tick(tom.white));
  return SHAPER::soft(shell + noise * level * HIT * tom.velocity);
}
