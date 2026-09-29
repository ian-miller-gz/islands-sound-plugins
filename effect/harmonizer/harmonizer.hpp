// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::HARMONIZER {

constexpr Whole VOICES = 4;

constexpr Whole MODE = 0;
constexpr Whole KEY = 1;
constexpr Whole RETUNE = 2;
constexpr Whole FORMANT = 3;
constexpr Whole DRY = 4;
constexpr Whole LEVEL = 5;
constexpr Whole PAN = LEVEL + VOICES;
constexpr Whole PARAMETERS = PAN + VOICES;

constexpr Whole RELATIVE = 0;
constexpr Whole ABSOLUTE = 1;
constexpr Whole MODES = 2;

constexpr Whole HIGHEST = 127;
constexpr Float MIDDLE = 60.0f;
constexpr Float BRISK = 20.0f;
constexpr Float SLOWEST = 400.0f;
constexpr Float LOUD = 0.7f;
constexpr Float WIDE = 0.5f;
constexpr Float NARROW = 0.25f;

inline constexpr STRING::Hot NAMES[] = {"Relative", "Absolute"};
static_assert(sizeof(NAMES) / sizeof(NAMES[0]) == MODES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Mode", "", Float(RELATIVE), Float(RELATIVE), Float(ABSOLUTE), MODES - 1,
   NAMES},
  {"Key", "", MIDDLE, 0, Float(HIGHEST), HIGHEST, nullptr},
  {"Retune", "ms", BRISK, 0, SLOWEST, 0, nullptr},
  {"Formant", "", 1, 0, 1, 1, nullptr},
  {"Dry", "", 1, 0, 1, 0, nullptr},
  {"Level1", "", LOUD, 0, 1, 0, nullptr},
  {"Level2", "", LOUD, 0, 1, 0, nullptr},
  {"Level3", "", LOUD, 0, 1, 0, nullptr},
  {"Level4", "", LOUD, 0, 1, 0, nullptr},
  {"Pan1", "", -WIDE, -1, 1, 0, nullptr},
  {"Pan2", "", WIDE, -1, 1, 0, nullptr},
  {"Pan3", "", -NARROW, -1, 1, 0, nullptr},
  {"Pan4", "", NARROW, -1, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::HARMONIZER
