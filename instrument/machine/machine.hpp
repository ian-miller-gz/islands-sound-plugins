// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "machine.rows.hpp"

namespace SOUND::PLUGINS::MACHINE {

enum Drum : Whole { KICK, SNARE, HAT, CLAP, TOM, CYMBAL, RIM, DRUMS };

constexpr Whole TUNE = 0;
constexpr Whole LEVEL = 1;
constexpr Whole KNOBS = 2;
constexpr Whole STANDING = CYMBAL;
constexpr Whole ACCENT = STANDING * KNOBS;
constexpr Whole TAIL = ACCENT + 1;

constexpr auto place(Whole drum, Whole knob) -> Whole {
  return drum * KNOBS + knob;
}

constexpr auto tail(Whole drum, Whole knob) -> Whole {
  return TAIL + (drum - STANDING) * KNOBS + knob;
}

inline constexpr Whole TONED[] = {SNARE, CLAP, TOM};

constexpr Whole DECAYS = tail(DRUMS, TUNE);
constexpr Whole ATTACK = DECAYS + DRUMS;
constexpr Whole TONES = ATTACK + 1;
constexpr Whole PANS = TONES + std::size(TONED);
constexpr Whole CURVE = PANS + DRUMS;

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == CURVE + 1);

constexpr Whole NONE = PARAMETERS;

struct Seat {
  Whole tune;
  Whole level;
  Whole decay;
  Whole timbre;
  Whole pan;
};

constexpr auto tone(Whole drum) -> Whole {
  for (Whole at = 0; at < std::size(TONED); ++at)
    if (TONED[at] == drum) return TONES + at;
  return drum == KICK ? ATTACK : NONE;
}

constexpr auto seat(Whole drum) -> Seat {
  const Flag standing = drum < STANDING;
  return {
    .tune = standing ? place(drum, TUNE) : tail(drum, TUNE),
    .level = standing ? place(drum, LEVEL) : tail(drum, LEVEL),
    .decay = DECAYS + drum,
    .timbre = tone(drum),
    .pan = PANS + drum};
}

inline constexpr Seat SEATS[DRUMS] = {seat(KICK), seat(SNARE), seat(HAT),
                                      seat(CLAP), seat(TOM),   seat(CYMBAL),
                                      seat(RIM)};

constexpr auto owner(Whole index) -> Whole {
  for (Whole drum = 0; drum < DRUMS; ++drum) {
    const Seat &held = SEATS[drum];
    if (
      index == held.tune || index == held.decay || index == held.timbre ||
      index == held.pan)
      return drum;
  }
  return DRUMS;
}

}  // namespace SOUND::PLUGINS::MACHINE
