// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "drawbar.indices.hpp"

namespace SOUND::DRAWBAR {

constexpr Float PULLED = 8;
constexpr Whole DEGREE = 8;
constexpr Float CENTS = 100;
constexpr Whole SCANS = 6;
constexpr Whole SPEEDS = 2;
constexpr Float THIRD = 1;
constexpr Float CHORUS = 6;
constexpr Float SLOW = 1;

inline constexpr STRING::Hot DEGREES[] = {"0", "1", "2", "3", "4",
                                          "5", "6", "7", "8"};
inline constexpr STRING::Hot HARMONICS[] = {"Second", "Third"};
inline constexpr STRING::Hot DECAYS[] = {"Fast", "Slow"};
inline constexpr STRING::Hot LEVELS[] = {"Normal", "Soft"};
inline constexpr STRING::Hot SCANNERS[] = {"Off", "V1", "V2", "V3",
                                           "C1",  "C2", "C3"};
inline constexpr STRING::Hot ROTORS[] = {"Off", "Slow", "Fast"};
static_assert(std::size(DEGREES) == DEGREE + 1);
static_assert(std::size(SCANNERS) == SCANS + 1);
static_assert(std::size(ROTORS) == SPEEDS + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"16'", "", PULLED, 0, PULLED, DEGREE, DEGREES},
  {"5 1/3'", "", PULLED, 0, PULLED, DEGREE, DEGREES},
  {"8'", "", PULLED, 0, PULLED, DEGREE, DEGREES},
  {"4'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"2 2/3'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"2'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"1 3/5'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"1 1/3'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"1'", "", 0, 0, PULLED, DEGREE, DEGREES},
  {"Percussion", "", 1, 0, 1, 1, nullptr},
  {"Harmonic", "", THIRD, 0, 1, 1, HARMONICS},
  {"Decay", "", 0, 0, 1, 1, DECAYS},
  {"Level", "", 0, 0, 1, 1, LEVELS},
  {"Click", "", 0.3f, 0, 1, 0, nullptr},
  {"Scanner", "", CHORUS, 0, SCANS, SCANS, SCANNERS},
  {"Rotary", "", SLOW, 0, SPEEDS, SPEEDS, ROTORS},
  {"Drive", "", 0, 0, 1, 0, nullptr},
  {"Balance", "", 0.5f, 0, 1, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::DRAWBAR
