// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "chip.indices.hpp"

namespace SOUND::CHIP {

constexpr Float LOUDEST = 15;
constexpr Whole LOUDNESSES = 15;
constexpr Float WIDEST = 3;
constexpr Whole WIDTHS = 3;
constexpr Float SHIFT = 24;
constexpr Whole SHIFTS = 48;
constexpr Whole INTERVALS = 24;
constexpr Float FRAMES = 60;
constexpr Float SLOWEST = 1;
constexpr Float FASTEST = 240;
constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;

inline constexpr STRING::Hot DIGITS[] = {
  "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12",
  "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

inline constexpr STRING::Hot DUTIES[] = {"12.5%", "25%", "50%", "75%"};

inline constexpr STRING::Hot MODES[] = {"Long", "Short"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Duty1", "", 2, 0, WIDEST, WIDTHS, DUTIES},
  {"Level1", "", LOUDEST, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Decay1", "", 4, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Sustain1", "", 10, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Duty2", "", 1, 0, WIDEST, WIDTHS, DUTIES},
  {"Level2", "", 0, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Decay2", "", 4, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Sustain2", "", 10, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Offset2", "st", 0, -SHIFT, SHIFT, SHIFTS, nullptr},
  {"Triangle", "", 0, 0, 1, 1, nullptr},
  {"Offset3", "st", -SEMITONES, -SHIFT, SHIFT, SHIFTS, nullptr},
  {"Mode", "", 0, 0, 1, 1, MODES},
  {"Period", "", 8, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Level4", "", 0, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Decay4", "", 1, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"Sustain4", "", 0, 0, LOUDEST, LOUDNESSES, DIGITS},
  {"First", "st", 0, 0, SHIFT, INTERVALS, DIGITS},
  {"Second", "st", 0, 0, SHIFT, INTERVALS, DIGITS},
  {"Speed", "Hz", FRAMES, SLOWEST, FASTEST, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::CHIP
