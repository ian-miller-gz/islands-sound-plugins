// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::SPRING {

constexpr Whole TENSION = 0;
constexpr Whole DECAY = 1;
constexpr Whole MIX = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tension", "", 0.5f, 0, 1, 0, nullptr},
  {"Decay", "s", 1.5f, 0.2f, 6, 0, nullptr},
  {"Mix", "", 0.3f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::SPRING
