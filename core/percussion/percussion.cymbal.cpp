// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SEMITONE = 100.0f;
constexpr Float PING = 3440.0f;
constexpr Float WASH = 7100.0f;
constexpr Float SIZZLE = 10500.0f;
constexpr Float CENTRES[PERCUSSION::TAIL::TAILS] = {PING, WASH, SIZZLE};
constexpr Float AIRS[PERCUSSION::TAIL::TAILS] = {0, 1.0f, 1.0f};
constexpr Float WIDTH = 1.0f;
constexpr Float LEVEL = 0;
constexpr Float LENGTHS[PERCUSSION::PLATE::PLATES][PERCUSSION::TAIL::TAILS] = {
  {1.0f, 0.4f, 0.6f}, {0.7f, 1.0f, 0.8f}, {1.4f, 0.15f, 0.1f}};
constexpr Float FLASH = 0.04f;
constexpr Float BALANCE = 2.0f;
constexpr Float ONE = 1.0f;
constexpr Float EDGE = 2000.0f;
constexpr Float HISS = 0.15f;
constexpr Float CURVE = 0;
constexpr Float TOUCH = 0.5f;
constexpr Float FADE = 0.005f;

void balance(PERCUSSION::Cymbal &cymbal, Float tone) {
  namespace TAIL = PERCUSSION::TAIL;
  cymbal.weights[TAIL::PING] = (ONE - tone) * BALANCE;
  cymbal.weights[TAIL::WASH] = ONE;
  cymbal.weights[TAIL::SIZZLE] = tone * BALANCE;
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate) {
  const Bronze bronze = {
    .tune = tune,
    .decay = decay,
    .tone = tone,
    .splash = splash,
    .edge = ::EDGE,
    .noise = ::HISS,
    .curve = ::CURVE,
    .touch = ::TOUCH};
  settle(cymbal, bronze, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Cymbal &cymbal, const Bronze &bronze, Whole rate) {
  const Float ratio = PHASE::ratio(bronze.tune * SEMITONE);
  settle(cymbal.metal, METALLIC, BANK, ratio, rate);
  for (Whole tail = 0; tail < TAIL::TAILS; ++tail) {
    FILTER::settle(
      cymbal.bands[tail], FILTER::BAND, CENTRES[tail], WIDTH, LEVEL, rate);
    cymbal.airs[tail] = bronze.noise * AIRS[tail];
  }
  for (Whole plate = 0; plate < PLATE::PLATES; ++plate)
    for (Whole tail = 0; tail < TAIL::TAILS; ++tail)
      shape(
        cymbal.tails[plate][tail], bronze.decay * LENGTHS[plate][tail], rate);
  shape(cymbal.flash, FLASH, rate);
  FILTER::settle(
    cymbal.edge, FILTER::HIGH, bronze.edge, FILTER::FLAT, LEVEL, rate);
  settle(cymbal.choke, ::FADE, rate);
  ::balance(cymbal, bronze.tone);
  cymbal.splash = bronze.splash;
  cymbal.curve = bronze.curve;
  cymbal.touch = bronze.touch;
}
