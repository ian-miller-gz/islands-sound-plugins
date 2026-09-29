// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::GRAIN {

constexpr Whole SIZE = 0;
constexpr Whole DENSITY = 1;
constexpr Whole PITCH = 2;
constexpr Whole SPRAY = 3;
constexpr Whole FEEDBACK = 4;
constexpr Whole MIX = 5;
constexpr Whole PARAMETERS = 6;

constexpr Float LONGEST = 500;
constexpr Float OCTAVES = 24;
constexpr Float WIDEST = 1000;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Size", "ms", 80, 10, LONGEST, 0, nullptr},
  {"Density", "Hz", 20, 1, 100, 0, nullptr},
  {"Pitch", "st", 0, -OCTAVES, OCTAVES, Whole(OCTAVES * 2), nullptr},
  {"Spray", "ms", 100, 0, WIDEST, 0, nullptr},
  {"Feedback", "", 0.2f, 0, 0.95f, 0, nullptr},
  {"Mix", "", 0.5f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::GRAIN
