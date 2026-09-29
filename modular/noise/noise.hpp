// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::NOISE {

constexpr Whole COLOUR = 0;
constexpr Whole GAIN = 1;
constexpr Whole GATE = 2;
constexpr Whole PARAMETERS = 3;

constexpr Whole WHITE = 0;
constexpr Whole PINK = 1;
constexpr Whole BROWN = 2;
constexpr Whole BLUE = 3;
constexpr Whole COLOURS = 4;

inline constexpr STRING::Hot LABELS[] = {"White", "Pink", "Brown", "Blue"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == COLOURS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Colour", "", WHITE, WHITE, BLUE, BLUE, LABELS},
  {"Gain", "", 0.25f, 0, 1, 0, nullptr},
  {"Gate", "", 1, 0, 1, 1, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::NOISE
