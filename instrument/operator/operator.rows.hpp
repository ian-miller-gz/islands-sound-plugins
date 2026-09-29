// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../core/table/table.hpp"
#include "operator.indices.hpp"
#include "operator.names.hpp"

namespace SOUND::OPERATOR {

constexpr Float ALGORITHMS = 32;
constexpr Float LOOPS = 7;
constexpr Float SHIFTS = 7;
constexpr Float TOP = 99;
constexpr Float LOWEST = 0.5f;
constexpr Float HIGHEST = 32;
constexpr Float SLOWEST = 0.05f;
constexpr Float FASTEST = 50;
constexpr Float LONGEST = 5;
constexpr Float OCTAVE = 1200;
constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;
constexpr Float SHAPES = 5;

inline constexpr STRING::Hot DIGITS[] = {
  "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
  "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
  "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32"};

inline constexpr STRING::Hot DETUNES[] = {"-7", "-6", "-5", "-4", "-3",
                                          "-2", "-1", "0",  "+1", "+2",
                                          "+3", "+4", "+5", "+6", "+7"};

inline constexpr STRING::Hot WAVES[] = {"Triangle", "Down", "Up",
                                        "Square",   "Sine", "Hold"};

inline constexpr CORE::TABLE::Row HEAD[] = {
  {"Algorithm", "", 1, 1, ALGORITHMS, Whole(ALGORITHMS) - 1, DIGITS + 1},
  {"Feedback", "", 0, 0, LOOPS, Whole(LOOPS), DIGITS}};

inline constexpr CORE::TABLE::Row FIELD[] = {
  {"", "", 1, LOWEST, HIGHEST, 0, nullptr},
  {"", "", 0, -SHIFTS, SHIFTS, Whole(SHIFTS) * 2, DETUNES},
  {"", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"", "", 30, 0, TOP, Whole(TOP), nullptr},
  {"", "", 20, 0, TOP, Whole(TOP), nullptr},
  {"", "", 50, 0, TOP, Whole(TOP), nullptr},
  {"", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"", "", 80, 0, TOP, Whole(TOP), nullptr},
  {"", "", 60, 0, TOP, Whole(TOP), nullptr},
  {"", "", 0, 0, TOP, Whole(TOP), nullptr}};
static_assert(std::size(FIELD) == FIELDS);

inline constexpr Float OUTPUTS[OPERATORS] = {TOP, 75, 0, 0, 0, 0};

inline constexpr CORE::TABLE::Row REST[] = {
  {"Speed", "Hz", 5, SLOWEST, FASTEST, 0, nullptr},
  {"Delay", "s", 0, 0, LONGEST, 0, nullptr},
  {"Wave", "", 0, 0, SHAPES, Whole(SHAPES), WAVES},
  {"Pitch", "ct", 0, 0, OCTAVE, 0, nullptr},
  {"Amplitude", "", 0, 0, 1, 0, nullptr},
  {"Sync", "", 1, 0, 1, 1, nullptr},
  {"Velocity", "", 0.5f, 0, 1, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(REST) == PARAMETERS - TAIL);

constexpr auto describe(Whole index) -> CORE::TABLE::Row {
  if (index < FIRST) return HEAD[index];
  if (index >= TAIL) return REST[index - TAIL];
  const Whole unit = (index - FIRST) / FIELDS;
  const Whole at = (index - FIRST) % FIELDS;
  CORE::TABLE::Row row = FIELD[at];
  row.name = NAMES[unit][at];
  if (at == OUTPUT) row.resting = OUTPUTS[unit];
  return row;
}

}  // namespace SOUND::OPERATOR
