// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../phase/phase.hpp"
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float BEND = 16.0f;
constexpr Float DROP = 0.03f;
constexpr Float DRIVE = 8.0f;
constexpr Float FLAT = 0;
constexpr Float STILL = 0;
constexpr Float LINEAR = 0;
constexpr Float FIXED = 0;
constexpr Float CEILING = 0.2f;
constexpr Float SIGH = 0.5f;
constexpr Float HUSH = 100.0f;
constexpr Float BASS = 80.0f;
constexpr Float TREBLE = 5000.0f;
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
    .click = TRANSIENT::NOISE,
    .sigh = STILL,
    .low = FLAT,
    .high = FLAT,
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
  shape(kick, batter.click, rate);
  FILTER::settle(kick.hush, FILTER::LOW, HUSH, rate);
  FILTER::settle(
    kick.low, FILTER::LOWSHELF, BASS, FILTER::FLAT, batter.low, rate);
  FILTER::settle(
    kick.high, FILTER::HIGHSHELF, TREBLE, FILTER::FLAT, batter.high, rate);
  kick.attack = batter.attack;
  kick.drive = ONE + batter.drive * DRIVE;
  kick.colour = batter.tone;
  kick.curve = batter.curve;
  kick.touch = batter.touch;
  kick.rate = rate;
  darken(kick);
}

void SOUND::PLUGINS::CORE::PERCUSSION::darken(Kick &kick) {
  const Float tone = touched(kick.colour, kick.velocity, kick.touch);
  FILTER::settle(kick.tone, FILTER::LOW, tone, kick.rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Kick &kick, Float velocity) {
  strike(kick, kick.hertz, velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Kick &kick) -> Float {
  if (!sounding(kick)) return 0;
  kick.body.base = kick.base * (ONE + kick.sigh * kick.breath);
  const Float body = tick(kick.body);
  const Float hushed = BLOCK::magnitude(FILTER::tick(kick.hush, body));
  kick.breath = hushed < ONE ? hushed : ONE;
  const Float drive = kick.drive;
  const auto clip = [drive](Float in) { return SHAPER::soft(in * drive); };
  const Float driven = SHAPER::tick(kick.oversampler, body + crack(kick), clip);
  const Float toned = FILTER::tick(kick.tone, driven);
  return FILTER::tick(kick.high, FILTER::tick(kick.low, toned));
}
