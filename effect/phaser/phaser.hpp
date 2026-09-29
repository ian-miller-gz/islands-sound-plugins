// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PHASER {

constexpr Whole STAGES = 0;
constexpr Whole RATE = 1;
constexpr Whole DEPTH = 2;
constexpr Whole FEEDBACK = 3;
constexpr Whole CENTRE = 4;
constexpr Whole PARAMETERS = 5;

constexpr Whole FEWEST = 4;
constexpr Whole STRIDE = 2;
constexpr Whole MOST = 8;

inline constexpr STRING::Hot COUNTS[] = {"Four", "Six", "Eight"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Stages", "", 0, 0, 2, 2, COUNTS},
  {"Rate", "Hz", 0.5f, 0.02f, 5, 0, nullptr},
  {"Depth", "", 0.7f, 0, 1, 0, nullptr},
  {"Feedback", "", 0.3f, 0, 0.9f, 0, nullptr},
  {"Centre", "Hz", 700, 100, 4000, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PHASER
