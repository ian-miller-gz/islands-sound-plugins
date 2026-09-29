// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::ENSEMBLE {

constexpr Whole VIOLIN = 0;
constexpr Whole VIOLA = 1;
constexpr Whole CELLO = 2;
constexpr Whole CONTRABASS = 3;
constexpr Whole ATTACK = 4;
constexpr Whole RELEASE = 5;
constexpr Whole MIX = 6;
constexpr Whole DEPTH = 7;
constexpr Whole CHORUS = 8;
constexpr Whole VIBRATO = 9;
constexpr Whole TUNE = 10;
constexpr Whole BEND = 11;
constexpr Whole VOLUME = 12;
constexpr Whole PARAMETERS = 13;

constexpr Float CENTS = 100;
constexpr Float SEMITONES = 12;
constexpr Float QUICKEST = 0.005f;
constexpr Float SLOWEST = 5;
constexpr Float LONGEST = 10;
constexpr Float LEAST = 0.05f;
constexpr Float SLOW = 2;
constexpr Float FAST = 3;
constexpr Float FASTEST = 10;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Violin", "", 0.8f, 0, 1, 0, nullptr},
  {"Viola", "", 0.5f, 0, 1, 0, nullptr},
  {"Cello", "", 0.4f, 0, 1, 0, nullptr},
  {"Contrabass", "", 0, 0, 1, 0, nullptr},
  {"Attack", "s", 0.15f, QUICKEST, SLOWEST, 0, nullptr},
  {"Release", "s", 1.2f, QUICKEST, LONGEST, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr},
  {"Depth", "", 1, 0, 1, 0, nullptr},
  {"Chorus", "Hz", 0.6f, LEAST, SLOW, 0, nullptr},
  {"Vibrato", "Hz", 6, FAST, FASTEST, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::ENSEMBLE
