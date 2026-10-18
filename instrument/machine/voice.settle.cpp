// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SEMITONE = 100.0f;

struct Voicing {
  Float tune = 0;
  Float closed = 0;
  Float snappy = 0;
  Float spread = 0;
};

constexpr Voicing VOICINGS[MACHINE::DRUMS] = {
  {.tune = 50},      {.tune = 185, .snappy = 0.7f},
  {.closed = 0.05f}, {.spread = 0.009f},
  {.tune = 100},     {},
  {.tune = 2500}};

struct Knobs {
  Float shift = 0;
  Float decay = 0;
  Float timbre = 0;
};

auto tuned(Float hertz, Float shift) -> Float {
  return hertz * CORE::PHASE::ratio(shift * SEMITONE);
}

using Settle =
  void (*)(MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs);

constexpr Settle SETTLES[MACHINE::DRUMS] = {
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.kick, tuned(made.tune, knobs.shift), knobs.decay, knobs.timbre,
      machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.snare, tuned(made.tune, knobs.shift), knobs.decay, made.snappy,
      knobs.timbre, machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.hat, knobs.shift, made.closed, knobs.decay, machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::settle(
      machine.clap, made.spread, knobs.decay, tuned(knobs.timbre, knobs.shift),
      machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.tom, tuned(made.tune, knobs.shift), knobs.decay, knobs.timbre,
      machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &, const Knobs &knobs) {
    CORE::PERCUSSION::HYBRID::settle(
      machine.cymbal, knobs.shift, knobs.decay, machine.rate);
  },
  [](MACHINE::Machine &machine, const Voicing &made, const Knobs &knobs) {
    CORE::PERCUSSION::settle(
      machine.rim, tuned(made.tune, knobs.shift), knobs.decay, machine.rate);
  }};

auto knobs(const MACHINE::Machine &machine, Whole drum) -> Knobs {
  const MACHINE::Seat &seat = MACHINE::SEATS[drum];
  const Float timbre =
    seat.timbre < MACHINE::PARAMETERS ? machine.rows[seat.timbre] : 0;
  return {machine.rows[seat.tune], machine.rows[seat.decay], timbre};
}

}  // namespace

void SOUND::PLUGINS::MACHINE::settle(Machine &machine, Whole drum) {
  if (drum >= DRUMS) return;
  machine.gains[drum] = CORE::STEREO::pan(machine.rows[SEATS[drum].pan]);
  ::SETTLES[drum](machine, ::VOICINGS[drum], ::knobs(machine, drum));
}

void SOUND::PLUGINS::MACHINE::settle(Machine &machine) {
  for (Whole drum = 0; drum < DRUMS; ++drum) settle(machine, drum);
}
