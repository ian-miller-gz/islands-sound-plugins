// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::VIBRATO {

constexpr Whole RATE = 0;
constexpr Whole DEPTH = 1;
constexpr Whole PARAMETERS = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rate", "Hz", 5, 0.1f, 14, 0, nullptr},
  {"Depth", "ct", 20, 0, 100, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::VIBRATO
