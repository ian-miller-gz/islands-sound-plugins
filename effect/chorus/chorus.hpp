// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::CHORUS {

constexpr Whole VOICES = 0;
constexpr Whole RATE = 1;
constexpr Whole DEPTH = 2;
constexpr Whole SPREAD = 3;
constexpr Whole MIX = 4;
constexpr Whole PARAMETERS = 5;

constexpr Whole FEWEST = 2;
constexpr Whole MOST = 3;

inline constexpr STRING::Hot COUNTS[] = {"Two", "Three"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Voices", "", 0, 0, 1, 1, COUNTS},
  {"Rate", "Hz", 0.8f, 0.05f, 5, 0, nullptr},
  {"Depth", "ms", 3, 0, 10, 0, nullptr},
  {"Spread", "", 0.5f, 0, 1, 0, nullptr},
  {"Mix", "", 0.5f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::CHORUS
