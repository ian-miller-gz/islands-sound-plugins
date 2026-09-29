// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::QUANTIZER {

constexpr Whole SCALE = 0;
constexpr Whole ROOT = 1;
constexpr Whole PARAMETERS = 2;

constexpr Whole SCALES = 6;
constexpr Whole DEGREES = 12;

inline constexpr STRING::Hot NAMES[] = {"Major",      "Minor", "Dorian",
                                        "Pentatonic", "Blues", "Chromatic"};
static_assert(sizeof(NAMES) / sizeof(NAMES[0]) == SCALES);

inline constexpr STRING::Hot KEYS[] = {"C",  "C#", "D",  "D#", "E",  "F",
                                       "F#", "G",  "G#", "A",  "A#", "B"};
static_assert(sizeof(KEYS) / sizeof(KEYS[0]) == DEGREES);

inline constexpr Flag MEMBERS[SCALES][DEGREES] = {
  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1}, {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0},
  {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0}, {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
  {1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Scale", "", 0, 0, SCALES - 1, SCALES - 1, NAMES},
  {"Root", "", 0, 0, DEGREES - 1, DEGREES - 1, KEYS}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::QUANTIZER
