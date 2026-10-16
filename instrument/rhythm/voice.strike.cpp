// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::strike;

using Strike = void (*)(RHYTHM::Box &box, Whole part, Float velocity);

constexpr Strike STRIKES[RHYTHM::VOICES] = {
  [](RHYTHM::Box &box, Whole, Float velocity) { strike(box.kick, velocity); },
  [](RHYTHM::Box &box, Whole, Float velocity) { strike(box.snare, velocity); },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.hat, part, velocity);
  },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.cymbal, part, velocity);
  },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.tom, part, velocity);
  },
  [](RHYTHM::Box &box, Whole, Float velocity) { strike(box.clap, velocity); },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.rim, part, velocity);
  },
  [](RHYTHM::Box &box, Whole, Float velocity) {
    strike(box.cowbell, velocity);
  }};

}  // namespace

void SOUND::PLUGINS::RHYTHM::strike(
  Box &box, Whole voice, Whole part, Float velocity) {
  if (voice < VOICES) ::STRIKES[voice](box, part, velocity);
}
