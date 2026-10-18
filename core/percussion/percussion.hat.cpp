// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SEMITONE = 100.0f;
constexpr Float SHEEN = 7100.0f;
constexpr Float WIDTH = 1.0f;
constexpr Float FLAT = 0;
constexpr Float FADE = 0.005f;
constexpr Float UNITY = 1.0f;
constexpr Float FULL = 1.0f;
constexpr Float HALF = 0.5f;

constexpr Float LEVELS[PERCUSSION::OPENING::OPENINGS] = {FULL, FULL, HALF};

void tuned(PERCUSSION::Hat &hat) {
  if (hat.rate == 0) return;
  FILTER::settle(
    hat.band, FILTER::BAND, SHEEN * hat.shade, WIDTH, FLAT, hat.rate);
}

void shut(PERCUSSION::Hat &hat, Whole opening, Float level) {
  PERCUSSION::close(hat.choke);
  hat.opening = opening;
  hat.shut.velocity = level * LEVELS[opening];
  ENVELOPE::strike(hat.shut.gate, hat.envelopes[opening], hat.shut.velocity);
}

void swing(PERCUSSION::Hat &hat, Whole opening, Float level) {
  PERCUSSION::open(hat.choke);
  hat.open.velocity = level * LEVELS[opening];
  ENVELOPE::strike(hat.open.gate, hat.envelopes[opening], hat.open.velocity);
}

using Stroking = void (*)(PERCUSSION::Hat &, Whole, Float);

constexpr Stroking STROKES[PERCUSSION::OPENING::OPENINGS] = {shut, swing, shut};

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Hat &hat, Float tune, Float closed, Float open, Float tone, Whole rate) {
  const Float ratio = PHASE::ratio(tune * SEMITONE);
  hat.rate = rate;
  settle(hat.metal, METALLIC, BANK, ratio, rate);
  ::tuned(hat);
  FILTER::settle(hat.high, FILTER::HIGH, tone, FILTER::FLAT, FLAT, rate);
  shape(hat.envelopes[OPENING::CLOSED], closed, rate);
  shape(hat.envelopes[OPENING::OPEN], open, rate);
  shape(hat.envelopes[OPENING::PEDAL], closed, rate);
  settle(hat.choke, FADE, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Hat &hat, const Setting &setting, Whole rate) {
  hat.setting = setting;
  if (setting.metal >= METAL::KINDS) hat.setting.metal = METAL::SQUARES;
  shape(hat.envelopes[OPENING::PEDAL], setting.pedal, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Hat &hat, Whole opening, Float velocity) {
  const Whole held = opening < OPENING::OPENINGS ? opening : OPENING::CLOSED;
  const Float level = struck(velocity);
  hat.shade = touched(::UNITY, level, hat.setting.touch);
  ::tuned(hat);
  ::STROKES[held](hat, held, level);
}
