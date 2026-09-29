// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::SLEW {

constexpr Whole RISE = 0;
constexpr Whole FALL = 1;
constexpr Whole PARAMETERS = 2;

constexpr Float SLOWEST = 5.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rise", "s", 0.001f, 0, SLOWEST, 0, nullptr},
  {"Fall", "s", 0.001f, 0, SLOWEST, 0, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::SLEW
