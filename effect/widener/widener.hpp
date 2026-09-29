// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::WIDENER {

constexpr Whole WIDTH = 0;
constexpr Whole MID = 1;
constexpr Whole SIDE = 2;
constexpr Whole BASS = 3;
constexpr Whole PARAMETERS = 4;

constexpr Float CUT = -24;
constexpr Float BOOST = 12;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Width", "", 1.5f, 0, 2, 0, nullptr},
  {"Mid", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Side", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Bass", "Hz", 120, 10, 500, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::WIDENER
