// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::RINGMOD {

constexpr Whole FREQUENCY = 0;
constexpr Whole WAVE = 1;
constexpr Whole MIX = 2;
constexpr Whole PARAMETERS = 3;

constexpr Whole SINE = 0;
constexpr Whole TRIANGLE = 1;
constexpr Whole SAW = 2;
constexpr Whole SQUARE = 3;
constexpr Whole WAVES = 4;

inline constexpr STRING::Hot SHAPES[] = {"Sine", "Triangle", "Saw", "Square"};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == WAVES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Frequency", "Hz", 440, 1, 5000, 0, nullptr},
  {"Wave", "", SINE, SINE, SQUARE, WAVES - 1, SHAPES},
  {"Mix", "", 1, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::RINGMOD
