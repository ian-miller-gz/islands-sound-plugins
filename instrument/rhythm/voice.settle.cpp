// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::settle;

constexpr Float KICK = 55;
constexpr Float SNARE = 180;
constexpr Float BODY = 0.5f;
constexpr Float CLICK = 0.4f;
constexpr Float DRIVE = 0.2f;
constexpr Float DULL = 4000;
constexpr Float BRIGHT = 3000;
constexpr Float SNAPPY = 0.6f;
constexpr Float CRACK = 0.25f;
constexpr Float CENTRE = 0;
constexpr Float CLOSED = 0.06f;
constexpr Float OPEN = 0.45f;
constexpr Float SHINE = 6000;
constexpr Float WASH = 1.5f;
constexpr Float BALANCE = 0.5f;
constexpr Float SPLASH = 0.5f;
constexpr Float DRUM = 90;
constexpr Float SKIN = 0.4f;
constexpr Float BEND = 0.3f;
constexpr Float CLEAR = 4000;
constexpr Float SPREAD = 0.011f;
constexpr Float CLAPPED = 0.3f;
constexpr Float PALM = 1100;
constexpr Float WOOD = 2500;
constexpr Float KNOCK = 0.06f;
constexpr Float PITCH = 0;
constexpr Float BELL = 0.35f;
constexpr Float EVEN = 0;

}  // namespace

void SOUND::PLUGINS::RHYTHM::settle(Box &box) {
  ::settle(box.kick, ::KICK, ::BODY, ::CLICK, ::DRIVE, ::DULL, box.rate);
  ::settle(box.snare, ::SNARE, ::BRIGHT, ::SNAPPY, ::CRACK, box.rate);
  ::settle(box.hat, ::CENTRE, ::CLOSED, ::OPEN, ::SHINE, box.rate);
  ::settle(box.cymbal, ::CENTRE, ::WASH, ::BALANCE, ::SPLASH, box.rate);
  ::settle(box.tom, ::DRUM, ::SKIN, ::BEND, ::CLEAR, box.rate);
  ::settle(box.clap, ::SPREAD, ::CLAPPED, ::PALM, box.rate);
  ::settle(box.rim, ::WOOD, ::KNOCK, box.rate);
  ::settle(box.cowbell, ::PITCH, ::BELL, box.rate);
  pace(box);
  if (box.rows[RUN] > 0) CORE::CLOCK::run(box.clock);
}

void SOUND::PLUGINS::RHYTHM::pace(Box &box) {
  const Pattern &chosen = pattern(Whole(box.rows[PATTERN]));
  const Float swing = chosen.division == SIXTEENTHS ? box.rows[SWING] : ::EVEN;
  CORE::CLOCK::settle(
    box.clock, box.rows[TEMPO], Float(chosen.division), swing, box.rate);
}
