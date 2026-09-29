// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::HALL {

constexpr Whole SIZE = 0;
constexpr Whole DECAY = 1;
constexpr Whole DAMP = 2;
constexpr Whole PREDELAY = 3;
constexpr Whole MIX = 4;
constexpr Whole PARAMETERS = 5;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Size", "", 1, 0.25f, 2, 0, nullptr},
  {"Decay", "s", 2.5f, 0.2f, 20, 0, nullptr},
  {"Damp", "", 0.4f, 0, 1, 0, nullptr},
  {"Predelay", "ms", 20, 0, 200, 0, nullptr},
  {"Mix", "", 0.3f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::HALL
