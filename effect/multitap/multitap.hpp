// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::MULTITAP {

constexpr Whole TAPS = 4;
constexpr Whole TIME = 0;
constexpr Whole LEVEL = TIME + TAPS;
constexpr Whole PAN = LEVEL + TAPS;
constexpr Whole FEEDBACK = PAN + TAPS;
constexpr Whole MIX = FEEDBACK + 1;
constexpr Whole PARAMETERS = MIX + 1;

constexpr Float SHORTEST = 1;
constexpr Float LONGEST = 2000;
constexpr Float LEFT = -1;
constexpr Float RIGHT = 1;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Time1", "ms", 125, SHORTEST, LONGEST, 0, nullptr},
  {"Time2", "ms", 250, SHORTEST, LONGEST, 0, nullptr},
  {"Time3", "ms", 375, SHORTEST, LONGEST, 0, nullptr},
  {"Time4", "ms", 500, SHORTEST, LONGEST, 0, nullptr},
  {"Level1", "", 0.8f, 0, 1, 0, nullptr},
  {"Level2", "", 0.6f, 0, 1, 0, nullptr},
  {"Level3", "", 0.45f, 0, 1, 0, nullptr},
  {"Level4", "", 0.3f, 0, 1, 0, nullptr},
  {"Pan1", "", -0.5f, LEFT, RIGHT, 0, nullptr},
  {"Pan2", "", 0.5f, LEFT, RIGHT, 0, nullptr},
  {"Pan3", "", -0.25f, LEFT, RIGHT, 0, nullptr},
  {"Pan4", "", 0.25f, LEFT, RIGHT, 0, nullptr},
  {"Feedback", "", 0.3f, 0, 0.95f, 0, nullptr},
  {"Mix", "", 0.35f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::MULTITAP
