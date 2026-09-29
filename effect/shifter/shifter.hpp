// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::SHIFTER {

constexpr Whole SHIFT = 0;
constexpr Whole FINE = 1;
constexpr Whole WINDOW = 2;
constexpr Whole MIX = 3;
constexpr Whole PARAMETERS = 4;

constexpr Float OCTAVES = 24;
constexpr Float WIDEST = 200;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Shift", "st", 7, -OCTAVES, OCTAVES, Whole(OCTAVES * 2), nullptr},
  {"Fine", "ct", 0, -100, 100, 0, nullptr},
  {"Window", "ms", 50, 10, WIDEST, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::SHIFTER
