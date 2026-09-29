// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

using PERCUSSION::PLATES;
using PERCUSSION::TAILS;

constexpr Float SEMITONE = 100.0f;
constexpr Float CENTRES[TAILS] = {3440.0f, 7100.0f};
constexpr Float WIDTH = 1.0f;
constexpr Float FLAT = 0;
constexpr Float LENGTHS[PLATES][TAILS] = {{1.0f, 0.4f}, {0.7f, 1.0f}};
constexpr Float SPLASHES[PLATES] = {0.3f, 1.0f};
constexpr Float FLASH = 0.04f;
constexpr Float BALANCE = 2.0f;
constexpr Float ONE = 1.0f;
constexpr Float GAIN = 4.0f;

auto sounding(const PERCUSSION::Cymbal &cymbal) -> Flag {
  for (const ENVELOPE::Gate &gate : cymbal.gates)
    if (ENVELOPE::sounding(gate)) return true;
  return false;
}

}  // namespace

void SOUND::CORE::PERCUSSION::settle(
  Cymbal &cymbal, Float tune, Float decay, Float tone, Float splash,
  Whole rate) {
  settle(cymbal.metal, METALLIC, BANK, PHASE::ratio(tune * SEMITONE), rate);
  for (Whole tail = 0; tail < TAILS; ++tail)
    FILTER::settle(
      cymbal.bands[tail], FILTER::BAND, CENTRES[tail], WIDTH, FLAT, rate);
  for (Whole plate = 0; plate < PLATES; ++plate)
    for (Whole tail = 0; tail < TAILS; ++tail)
      shape(cymbal.tails[plate][tail], decay * LENGTHS[plate][tail], rate);
  shape(cymbal.flash, FLASH, rate);
  cymbal.weights[PING] = (ONE - tone) * BALANCE;
  cymbal.weights[WASH] = tone * BALANCE;
  cymbal.splash = splash;
}

void SOUND::CORE::PERCUSSION::strike(
  Cymbal &cymbal, Whole plate, Float velocity) {
  cymbal.plate = plate < PLATES ? plate : CRASH;
  cymbal.velocity = struck(velocity);
  for (Whole tail = 0; tail < TAILS; ++tail)
    ENVELOPE::strike(
      cymbal.gates[tail], cymbal.tails[cymbal.plate][tail], cymbal.velocity);
  ENVELOPE::strike(cymbal.burst, cymbal.flash, cymbal.velocity);
}

auto SOUND::CORE::PERCUSSION::tick(Cymbal &cymbal) -> Float {
  if (!::sounding(cymbal)) return 0;
  const Float ring = tick(cymbal.metal);
  Float bands[TAILS] = {};
  Float sum = 0;
  for (Whole tail = 0; tail < TAILS; ++tail) {
    const ENVELOPE::Envelope &envelope = cymbal.tails[cymbal.plate][tail];
    const Float level = ENVELOPE::tick(cymbal.gates[tail], envelope);
    bands[tail] = FILTER::tick(cymbal.bands[tail], ring);
    sum += bands[tail] * level * cymbal.weights[tail];
  }
  const Float flash = ENVELOPE::tick(cymbal.burst, cymbal.flash);
  const Float splash = flash * cymbal.splash * SPLASHES[cymbal.plate];
  return SHAPER::soft((sum + bands[WASH] * splash) * cymbal.velocity * GAIN);
}
