// SPDX-License-Identifier: AGPL-3.0-or-later
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

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Snare &snare, Float tune, Float tone, Float snappy, Float decay, Whole rate) {
  settle(snare.low, tune, decay * BODY, rate);
  settle(snare.high, tune * DETUNE, decay * BODY, rate);
  FILTER::settle(snare.band, FILTER::BAND, tone, WIDTH, FLAT, rate);
  FILTER::settle(snare.floor, FILTER::HIGH, FLOOR, rate);
  shape(snare.rattle, decay, rate);
  snare.snappy = snappy;
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Snare &snare, Float velocity) {
  snare.velocity = struck(velocity);
  strike(snare.low, snare.velocity);
  strike(snare.high, snare.velocity * UPPER);
  ENVELOPE::strike(snare.gate, snare.rattle, snare.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Snare &snare) -> Float {
  const Float tones = (tick(snare.low) + tick(snare.high)) * TONES;
  const Float banded = FILTER::tick(snare.band, NOISE::tick(snare.white));
  const Float noise = FILTER::tick(snare.floor, banded);
  const Float level = ENVELOPE::tick(snare.gate, snare.rattle);
  return SHAPER::soft(tones + noise * level * snare.snappy * snare.velocity);
}
