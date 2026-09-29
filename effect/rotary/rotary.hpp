// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::ROTARY {

constexpr Whole SPEED = 0;
constexpr Whole CROSSOVER = 1;
constexpr Whole HORN = 2;
constexpr Whole DRUM = 3;
constexpr Whole INERTIA = 4;
constexpr Whole DISTANCE = 5;
constexpr Whole PARAMETERS = 6;

inline constexpr STRING::Hot SPEEDS[] = {"Slow", "Fast"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Speed", "", 0, 0, 1, 1, SPEEDS},
  {"Crossover", "Hz", 800, 200, 3000, 0, nullptr},
  {"Horn", "Hz", 6.7f, 3, 10, 0, nullptr},
  {"Drum", "Hz", 5.7f, 3, 10, 0, nullptr},
  {"Inertia", "s", 3, 0.1f, 10, 0, nullptr},
  {"Distance", "m", 0.5f, 0.1f, 3, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::ROTARY
