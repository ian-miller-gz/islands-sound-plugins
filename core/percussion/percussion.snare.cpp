// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float DETUNE = 1.8333f;
constexpr Float RING = 0.6f;
constexpr Float FLOOR = 500.0f;
constexpr Float OCTAVE = 12.0f;
constexpr Float ONE = 1.0f;
constexpr Float PING = 4.6f;
constexpr Float PINGING = 0.15f;
constexpr Float CLICK = 8.0f;
constexpr Float CLICKING = 0.004f;

auto deepened(Float bend) -> Float { return std::exp2(bend / OCTAVE) - ONE; }

auto rung(const PERCUSSION::SNARE::Setting &setting, Float length) -> Float {
  return setting.decay * RING * length;
}

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
  const Float seconds = ::rung(setting, ONE);
  ::tune(snare.low, setting.tune, seconds, setting, rate);
  ::tune(snare.high, setting.tune * DETUNE, seconds, setting, rate);
  for (Whole at = 0; at < SNARE::MODES; ++at)
    ::tune(
      snare.modes[at], setting.tune * SNARE::RATIOS[at],
      seconds / SNARE::RATIOS[at], setting, rate);
  settle(snare.ping, setting.tune * ::PING, setting.decay * ::PINGING, rate);
  settle(snare.click, setting.tune * ::CLICK, ::CLICKING, rate);
  FILTER::settle(
    snare.band, FILTER::BAND, setting.tone, SNARE::WIDTH, SNARE::FLAT, rate);
  FILTER::settle(snare.floor, FILTER::HIGH, ::FLOOR, rate);
  shape(snare.rattle, setting.decay, rate);
  snare.setting = setting;
  snare.rate = rate;
  snare.hold = Whole(ENVELOPE::frames(setting.hold, rate));
}

void SOUND::PLUGINS::CORE::PERCUSSION::ring(Snare &snare, Float length) {
  const Float seconds = ::rung(snare.setting, length);
  snare.low.body.damp = fall(seconds, snare.rate);
  snare.high.body.damp = snare.low.body.damp;
  for (Whole at = 0; at < SNARE::MODES; ++at)
    snare.modes[at].body.damp = fall(seconds / SNARE::RATIOS[at], snare.rate);
}
