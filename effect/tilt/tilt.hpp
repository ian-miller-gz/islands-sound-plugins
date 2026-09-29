// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::TILT {

constexpr Whole PIVOT = 0;
constexpr Whole TILT = 1;
constexpr Whole GAIN = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Pivot", "Hz", 1000, 100, 10000, 0, nullptr},
  {"Tilt", "dB", 0, -12, 12, 0, nullptr},
  {"Gain", "dB", 0, -12, 12, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::TILT
