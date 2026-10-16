// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SEMITONE = 100.0f;
constexpr Float PING = 3440.0f;
constexpr Float WASH = 7100.0f;
constexpr Float SIZZLE = 10500.0f;
constexpr Float CENTRES[PERCUSSION::TAIL::TAILS] = {PING, WASH, SIZZLE};
constexpr Float WIDTH = 1.0f;
constexpr Float FLAT = 0;
constexpr Float LENGTHS[PERCUSSION::PLATE::PLATES][PERCUSSION::TAIL::TAILS] = {
  {1.0f, 0.4f, 0.6f}, {0.7f, 1.0f, 0.8f}, {1.4f, 0.15f, 0.1f}};
constexpr Float FLASH = 0.04f;
constexpr Float BALANCE = 2.0f;
constexpr Float ONE = 1.0f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate) {
  settle(cymbal.metal, METALLIC, BANK, PHASE::ratio(tune * SEMITONE), rate);
  for (Whole tail = 0; tail < TAIL::TAILS; ++tail)
    FILTER::settle(
      cymbal.bands[tail], FILTER::BAND, CENTRES[tail], WIDTH, FLAT, rate);
  for (Whole plate = 0; plate < PLATE::PLATES; ++plate)
    for (Whole tail = 0; tail < TAIL::TAILS; ++tail)
      shape(cymbal.tails[plate][tail], decay * LENGTHS[plate][tail], rate);
  shape(cymbal.flash, FLASH, rate);
  cymbal.weights[TAIL::PING] = (ONE - tone) * BALANCE;
  cymbal.weights[TAIL::WASH] = ONE;
  cymbal.weights[TAIL::SIZZLE] = tone * BALANCE;
  cymbal.splash = splash;
}
