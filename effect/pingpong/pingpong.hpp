// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::PINGPONG {

constexpr Whole TIME = 0;
constexpr Whole FEEDBACK = 1;
constexpr Whole WIDTH = 2;
constexpr Whole MIX = 3;
constexpr Whole PARAMETERS = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Time", "ms", 375, 10, 1000, 0, nullptr},
  {"Feedback", "", 0.5f, 0, 0.95f, 0, nullptr},
  {"Width", "", 1, 0, 1, 0, nullptr},
  {"Mix", "", 0.35f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::PINGPONG
