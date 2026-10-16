// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float LENGTH = 0.12f;
constexpr PERCUSSION::HYBRID::Colour SHEEN = {.band = 8000.0f, .edge = 6000.0f};
constexpr Float GAIN = 1.5f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::build(Hat &hat, Whole rate) {
  build(hat.loop, ::LENGTH, ::SHEEN, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(
  Hat &hat, Float tune, Float closed, Float open, Whole rate) {
  settle(hat.loop, tune);
  shape(hat.envelopes[OPENING::CLOSED], closed, rate);
  shape(hat.envelopes[OPENING::OPEN], open, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(
  Hat &hat, Whole opening, Float velocity) {
  hat.opening = opening < OPENING::OPENINGS ? opening : OPENING::CLOSED;
  hat.velocity = struck(velocity);
  strike(hat.loop);
  ENVELOPE::strike(hat.gate, hat.envelopes[hat.opening], hat.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Hat &hat) -> Float {
  if (!sounding(hat)) return 0;
  const Float sample = tick(hat.loop);
  const Float level = ENVELOPE::tick(hat.gate, hat.envelopes[hat.opening]);
  return SHAPER::soft(sample * level * hat.velocity * ::GAIN);
}
