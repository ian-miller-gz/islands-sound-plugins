// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::TUNER {

constexpr Whole SCALE = 0;
constexpr Whole ROOT = 1;
constexpr Whole RETUNE = 2;
constexpr Whole HUMANISE = 3;
constexpr Whole FORMANT = 4;
constexpr Whole SHIFT = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole CHROMATIC = 0;
constexpr Whole MAJOR = 1;
constexpr Whole MINOR = 2;
constexpr Whole DORIAN = 3;
constexpr Whole PENTATONIC = 4;
constexpr Whole BLUES = 5;
constexpr Whole SCALES = 6;

constexpr Whole KEYS = 12;
constexpr Float OCTAVE = 12.0f;
constexpr Float BRISK = 20.0f;
constexpr Float SLOWEST = 400.0f;

inline constexpr STRING::Hot NAMES[] = {"Chromatic", "Major",      "Minor",
                                        "Dorian",    "Pentatonic", "Blues"};
static_assert(sizeof(NAMES) / sizeof(NAMES[0]) == SCALES);

inline constexpr Whole MASKS[] = {0b111111111111, 0b101010110101,
                                  0b010110101101, 0b011010101101,
                                  0b001010010101, 0b010011101001};
static_assert(sizeof(MASKS) / sizeof(MASKS[0]) == SCALES);

inline constexpr STRING::Hot NOTES[] = {"C",  "C#", "D",  "D#", "E",  "F",
                                        "F#", "G",  "G#", "A",  "A#", "B"};
static_assert(sizeof(NOTES) / sizeof(NOTES[0]) == KEYS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Scale", "", Float(CHROMATIC), Float(CHROMATIC), Float(BLUES), SCALES - 1,
   NAMES},
  {"Root", "", 0, 0, Float(KEYS - 1), KEYS - 1, NOTES},
  {"Retune", "ms", BRISK, 0, SLOWEST, 0, nullptr},
  {"Humanise", "", 0, 0, 1, 0, nullptr},
  {"Formant", "", 1, 0, 1, 1, nullptr},
  {"Shift", "st", 0, -OCTAVE, OCTAVE, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::TUNER
