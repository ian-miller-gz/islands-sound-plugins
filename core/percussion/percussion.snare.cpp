// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float DETUNE = 1.8333f;
constexpr Float BODY = 0.6f;
constexpr Float UPPER = 0.6f;
constexpr Float TONES = 0.7f;
constexpr Float WIDTH = 0.8f;
constexpr Float FLOOR = 500.0f;
constexpr Float FLAT = 0;
constexpr Float OCTAVE = 12.0f;
constexpr Float ONE = 1.0f;

auto deepened(Float bend) -> Float { return std::exp2(bend / OCTAVE) - ONE; }

void tune(
  PERCUSSION::Sweep &sweep, Float hertz, Float seconds,
  const PERCUSSION::SNARE::Setting &setting, Whole rate) {
  PERCUSSION::settle(
    sweep, seconds, setting.drop, deepened(setting.bend), rate);
  sweep.base = PERCUSSION::turn(hertz, rate);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Snare &snare, Float tune, Float tone, Float snappy, Float decay, Whole rate) {
  settle(
    snare,
    SNARE::Setting{
      .tune = tune, .tone = tone, .snappy = snappy, .decay = decay},
    rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Snare &snare, const SNARE::Setting &setting, Whole rate) {
  const Float seconds = setting.decay * BODY;
  ::tune(snare.low, setting.tune, seconds, setting, rate);
  ::tune(snare.high, setting.tune * DETUNE, seconds, setting, rate);
  FILTER::settle(snare.band, FILTER::BAND, setting.tone, WIDTH, FLAT, rate);
  FILTER::settle(snare.floor, FILTER::HIGH, ::FLOOR, rate);
  shape(snare.rattle, setting.decay, rate);
  snare.setting = setting;
  snare.rate = rate;
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Snare &snare, Float velocity) {
  snare.velocity = struck(velocity);
  strike(snare.low, snare.low.base, snare.velocity);
  strike(snare.high, snare.high.base, snare.velocity * UPPER);
  ENVELOPE::strike(snare.gate, snare.rattle, snare.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Snare &snare) -> Float {
  if (!sounding(snare)) return 0;
  const Float tones = (tick(snare.low) + tick(snare.high)) * TONES;
  const Float banded = FILTER::tick(snare.band, NOISE::tick(snare.white));
  const Float noise = FILTER::tick(snare.floor, banded);
  const Float level = ENVELOPE::tick(snare.gate, snare.rattle);
  const Float rattle = noise * level * snare.setting.snappy * snare.velocity;
  return SHAPER::soft(tones + rattle);
}
