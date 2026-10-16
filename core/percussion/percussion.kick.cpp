// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../phase/phase.hpp"
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float BEND = 16.0f;
constexpr Float DROP = 0.03f;
constexpr Float SNAP = 0.005f;
constexpr Float CRISP = 2000.0f;
constexpr Float DRIVE = 8.0f;
constexpr Float FLAT = 0;
constexpr Float STILL = 0;
constexpr Float LINEAR = 0;
constexpr Float FIXED = 0;
constexpr Float CEILING = 0.2f;
constexpr Float SIGH = 0.5f;
constexpr Float HUSH = 100.0f;
constexpr Float SEMITONE = PHASE::CENTS / PHASE::OCTAVE;

void bound(PERCUSSION::Kick &kick, Float depth, Whole rate) {
  kick.top = Float(rate) * CEILING / ((ONE + depth) * (ONE + SIGH));
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Kick &kick, Float tune, Float decay, Float attack, Float drive, Float tone,
  Whole rate) {
  const Batter batter = {
    .tune = tune,
    .decay = decay,
    .attack = attack,
    .drive = drive,
    .tone = tone,
    .bend = BEND,
    .drop = DROP,
    .slope = FLAT,
    .sigh = STILL,
    .curve = LINEAR,
    .touch = FIXED};
  settle(kick, batter, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Kick &kick, const Batter &batter, Whole rate) {
  const Float depth = PHASE::ratio(batter.bend * SEMITONE) - ONE;
  settle(kick.body, batter.decay, batter.drop, depth, batter.slope, rate);
  ::bound(kick, depth, rate);
  kick.hertz = batter.tune;
  kick.sigh = batter.sigh * SIGH;
  shape(kick.snap, SNAP, rate);
  FILTER::settle(kick.crisp, FILTER::HIGH, CRISP, rate);
  FILTER::settle(kick.hush, FILTER::LOW, HUSH, rate);
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
  kick.body.base = kick.base * (ONE + kick.sigh * kick.breath);
  const Float body = tick(kick.body);
  const Float hushed = BLOCK::magnitude(FILTER::tick(kick.hush, body));
  kick.breath = hushed < ONE ? hushed : ONE;
  const Float snap = ENVELOPE::tick(kick.click, kick.snap);
  const Float noise = FILTER::tick(kick.crisp, NOISE::tick(kick.white));
  const Float crack = noise * snap * kick.attack * kick.velocity;
  const Float driven = SHAPER::soft((body + crack) * kick.drive);
  return FILTER::tick(kick.tone, driven);
}
