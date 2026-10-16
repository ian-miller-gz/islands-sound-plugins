// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

void SOUND::PLUGINS::CORE::PERCUSSION::darken(Kick &kick) {
  const Float tone = touched(kick.colour, kick.velocity, kick.touch);
  FILTER::settle(kick.tone, FILTER::LOW, tone, kick.rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Kick &kick, Float velocity) {
  strike(kick, kick.hertz, velocity);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Kick &kick, Float hertz, Float velocity) {
  const Float held = hertz > kick.top ? kick.top : hertz;
  kick.base = turn(held, kick.rate);
  kick.velocity = struck(velocity, kick.curve);
  darken(kick);
  ENVELOPE::strike(kick.click, kick.snap, kick.velocity);
  kick.breath = 0;
  strike(kick.body, kick.base, kick.velocity);
}
