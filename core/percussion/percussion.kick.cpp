// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float BEND = 1.5f;
constexpr Float SWEEP = 0.03f;
constexpr Float SNAP = 0.005f;
constexpr Float CRISP = 2000.0f;
constexpr Float DRIVE = 8.0f;
constexpr Float LINEAR = 0;
constexpr Float FIXED = 0;
constexpr Float CEILING = 0.2f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Kick &kick, Float tune, Float decay, Float attack, Float drive, Float tone,
  Whole rate) {
  settle(
    kick,
    Batter{
      .tune = tune,
      .decay = decay,
      .attack = attack,
      .drive = drive,
      .tone = tone,
      .curve = LINEAR,
      .touch = FIXED},
    rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Kick &kick, const Batter &batter, Whole rate) {
  settle(kick.body, batter.tune, batter.decay, rate);
  kick.hertz = batter.tune;
  kick.top = Float(rate) * CEILING / (ONE + BEND);
  shape(kick.sweep, SWEEP, rate);
  shape(kick.snap, SNAP, rate);
  FILTER::settle(kick.crisp, FILTER::HIGH, CRISP, rate);
  kick.attack = batter.attack;
  kick.drive = ONE + batter.drive * DRIVE;
  kick.colour = batter.tone;
  kick.curve = batter.curve;
  kick.touch = batter.touch;
  kick.rate = rate;
  darken(kick);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Kick &kick) -> Float {
  if (!sounding(kick)) return 0;
  const Float sweep = ENVELOPE::tick(kick.bend, kick.sweep);
  kick.body.turn = kick.base * (ONE + BEND * sweep);
  const Float body = tick(kick.body);
  const Float snap = ENVELOPE::tick(kick.click, kick.snap);
  const Float noise = FILTER::tick(kick.crisp, NOISE::tick(kick.white));
  const Float crack = noise * snap * kick.attack * kick.velocity;
  const Float driven = SHAPER::soft((body + crack) * kick.drive);
  return FILTER::tick(kick.tone, driven);
}
