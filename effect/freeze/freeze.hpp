// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::FREEZE {

constexpr Whole HOLD = 0;
constexpr Whole SIZE = 1;
constexpr Whole DECAY = 2;
constexpr Whole MIX = 3;
constexpr Whole PARAMETERS = 4;

constexpr Float LONGEST = 1000;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Hold", "", 0, 0, 1, 1, nullptr},
  {"Size", "ms", 250, 20, LONGEST, 0, nullptr},
  {"Decay", "s", 20, 0.5f, 60, 0, nullptr},
  {"Mix", "", 0.5f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::FREEZE
