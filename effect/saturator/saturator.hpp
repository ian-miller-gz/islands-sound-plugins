// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::SATURATOR {

constexpr Whole DRIVE = 0;
constexpr Whole BIAS = 1;
constexpr Whole HISS = 2;
constexpr Whole LOW = 3;
constexpr Whole HIGH = 4;
constexpr Whole MIX = 5;
constexpr Whole PARAMETERS = 6;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Drive", "dB", 6, 0, 24, 0, nullptr},
  {"Bias", "", 0.1f, 0, 0.5f, 0, nullptr},
  {"Hiss", "", 0, 0, 1, 0, nullptr},
  {"Low", "Hz", 30, 10, 200, 0, nullptr},
  {"High", "Hz", 12000, 2000, 20000, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::SATURATOR
