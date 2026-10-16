// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float LOUDS[PERCUSSION::PLATE::PLATES][PERCUSSION::TAIL::TAILS] = {
  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.6f, 0.6f, 0}};
constexpr Float SPLASHES[PERCUSSION::PLATE::PLATES] = {0.3f, 1.0f, 0};

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Cymbal &cymbal, Whole plate, Float velocity) {
  cymbal.plate = plate < PLATE::PLATES ? plate : PLATE::CRASH;
  cymbal.velocity = struck(velocity, cymbal.curve);
  for (Whole tail = 0; tail < TAIL::TAILS; ++tail) {
    cymbal.gains[tail] = LOUDS[cymbal.plate][tail];
    ENVELOPE::strike(
      cymbal.gates[tail], cymbal.tails[cymbal.plate][tail], cymbal.velocity);
  }
  cymbal.gains[TAIL::SIZZLE] *= ENVELOPE::weigh(cymbal.velocity, cymbal.touch);
  cymbal.flare = SPLASHES[cymbal.plate];
  ENVELOPE::strike(cymbal.burst, cymbal.flash, cymbal.velocity);
  open(cymbal.choke);
}

void SOUND::PLUGINS::CORE::PERCUSSION::lift(Cymbal &cymbal) {
  close(cymbal.choke);
}
