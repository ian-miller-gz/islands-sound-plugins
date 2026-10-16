// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TOP = 1.9f;

void hit(PERCUSSION::Tom &tom, Float base, Float velocity) {
  tom.velocity = PERCUSSION::struck(velocity, tom.curve);
  PERCUSSION::strike(tom.sweep, base, tom.velocity);
  for (PERCUSSION::Resonator &mode : tom.modes)
    PERCUSSION::strike(mode, tom.velocity);
  const Float stick = PERCUSSION::touched(tom.colour, tom.velocity, tom.touch);
  FILTER::settle(tom.tone, FILTER::LOW, stick, tom.rate);
  ENVELOPE::strike(tom.hit, tom.skin, tom.velocity);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Tom &tom, Whole height, Float velocity) {
  const Whole at = height < HEIGHT::HEIGHTS ? height : HEIGHT::MID;
  ::hit(tom, tom.bases[at], velocity);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Tom &tom, Float hertz, Float velocity) {
  const Float base = turn(hertz, tom.rate);
  const Float top = TOP / (ONE + tom.sweep.depth);
  ::hit(tom, base < top ? base : top, velocity);
}
