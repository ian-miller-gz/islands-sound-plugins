// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float SEMITONE = 100.0f;
constexpr Float SHEEN = 7100.0f;
constexpr Float WIDTH = 1.0f;
constexpr Float FLAT = 0;
constexpr Float GAIN = 4.0f;

}  // namespace

void SOUND::CORE::PERCUSSION::settle(
  Hat &hat, Float tune, Float closed, Float open, Float tone, Whole rate) {
  const Float ratio = PHASE::ratio(tune * SEMITONE);
  settle(hat.metal, METALLIC, BANK, ratio, rate);
  FILTER::settle(hat.band, FILTER::BAND, SHEEN, WIDTH, FLAT, rate);
  FILTER::settle(hat.high, FILTER::HIGH, tone, FILTER::FLAT, FLAT, rate);
  shape(hat.envelopes[CLOSED], closed, rate);
  shape(hat.envelopes[OPEN], open, rate);
}

void SOUND::CORE::PERCUSSION::strike(Hat &hat, Whole opening, Float velocity) {
  hat.opening = opening < OPENINGS ? opening : CLOSED;
  hat.velocity = struck(velocity);
  ENVELOPE::strike(hat.gate, hat.envelopes[hat.opening], hat.velocity);
}

auto SOUND::CORE::PERCUSSION::tick(Hat &hat) -> Float {
  if (!ENVELOPE::sounding(hat.gate)) return 0;
  const Float ring = FILTER::tick(hat.band, tick(hat.metal));
  const Float level = ENVELOPE::tick(hat.gate, hat.envelopes[hat.opening]);
  const Float out = FILTER::tick(hat.high, ring * level * hat.velocity);
  return SHAPER::soft(out * GAIN);
}
