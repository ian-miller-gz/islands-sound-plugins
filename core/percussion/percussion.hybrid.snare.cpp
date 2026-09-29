// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

constexpr PERCUSSION::HYBRID::Bend DROP = {
  .depth = 0.6f, .fast = 0.008f, .knee = 0.15f, .slow = 0.05f};
constexpr Float DETUNE = 1.8f;
constexpr Float SHORT = 0.5f;
constexpr Float SECOND = 0.6f;
constexpr Float SHELL = 0.8f;
constexpr Float FLOOR = 900.0f;

}  // namespace

void SOUND::CORE::PERCUSSION::HYBRID::settle(
  Snare &snare, Float tune, Float decay, Float snappy, Float tone, Whole rate) {
  settle(snare.low, DROP, rate);
  settle(snare.high, DROP, rate);
  snare.lower = pace(tune, rate);
  snare.upper = pace(tune * DETUNE, rate);
  shape(snare.body, decay * SHORT, rate);
  shape(snare.rattle, decay, rate);
  FILTER::settle(snare.dark, FILTER::LOW, tone, rate);
  FILTER::settle(snare.floor, FILTER::HIGH, FLOOR, rate);
  snare.snappy = snappy;
}

void SOUND::CORE::PERCUSSION::HYBRID::strike(Snare &snare, Float velocity) {
  snare.velocity = struck(velocity);
  strike(snare.low, snare.lower);
  strike(snare.high, snare.upper);
  ENVELOPE::strike(snare.tone, snare.body, snare.velocity);
  ENVELOPE::strike(snare.snap, snare.rattle, snare.velocity);
}

auto SOUND::CORE::PERCUSSION::HYBRID::tick(Snare &snare) -> Float {
  if (!ENVELOPE::sounding(snare.snap)) return 0;
  const Float heads = tick(snare.low) + tick(snare.high) * SECOND;
  const Float shell = heads * ENVELOPE::tick(snare.tone, snare.body) * SHELL;
  const Float dark = FILTER::tick(snare.dark, NOISE::tick(snare.white));
  const Float noise = FILTER::tick(snare.floor, dark);
  const Float level = ENVELOPE::tick(snare.snap, snare.rattle) * snare.snappy;
  return SHAPER::soft((shell + noise * level) * snare.velocity);
}
