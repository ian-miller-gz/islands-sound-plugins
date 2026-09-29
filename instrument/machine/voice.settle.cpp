// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SEMITONE = 100.0f;

struct Voicing {
  Float tune = 0;
  Float decay = 0;
  Float open = 0;
  Float tone = 0;
  Float attack = 0;
  Float snappy = 0;
  Float spread = 0;
};

constexpr Voicing VOICINGS[MACHINE::DRUMS] = {
  {.tune = 50, .decay = 0.45f, .attack = 0.5f},
  {.tune = 185, .decay = 0.25f, .tone = 7000, .snappy = 0.7f},
  {.decay = 0.05f, .open = 0.35f},
  {.decay = 0.35f, .tone = 1300, .spread = 0.009f},
  {.tune = 100, .decay = 0.45f}};

auto tuned(Float hertz, Float shift) -> Float {
  return hertz * CORE::PHASE::ratio(shift * SEMITONE);
}

using Settle =
  void (*)(MACHINE::Machine &machine, const Voicing &made, Float shift);

constexpr Settle SETTLES[MACHINE::DRUMS] = {
  [](MACHINE::Machine &machine, const Voicing &made, Float shift) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.kick, tuned(made.tune, shift), made.decay, made.attack,
      machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, Float shift) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.snare, tuned(made.tune, shift), made.decay, made.snappy,
      made.tone, machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, Float shift) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.hat, shift, made.decay, made.open, machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      machine.clap, made.spread, made.decay, tuned(made.tone, shift),
      machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, Float shift) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.tom, tuned(made.tune, shift), made.decay, machine.rate);
  }};

}  // namespace

void SOUND::PLUGINS::MACHINE::settle(Machine &machine, Whole drum) {
  if (drum >= DRUMS) return;
  ::SETTLES[drum](machine, ::VOICINGS[drum], machine.rows[place(drum, TUNE)]);
}

void SOUND::PLUGINS::MACHINE::settle(Machine &machine) {
  for (Whole drum = 0; drum < DRUMS; ++drum) settle(machine, drum);
}
