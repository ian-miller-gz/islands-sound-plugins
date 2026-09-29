// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::BUS {

constexpr Whole ATTACK = 0;
constexpr Whole SUSTAIN = 1;
constexpr Whole THRESHOLD = 2;
constexpr Whole RATIO = 3;
constexpr Whole MIX = 4;
constexpr Whole DRIVE = 5;
constexpr Whole TILT = 6;
constexpr Whole OUTPUT = 7;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Attack", "dB", 3, -12, 12, 0, nullptr},
  {"Sustain", "dB", -2, -12, 12, 0, nullptr},
  {"Threshold", "dB", -18, -40, 0, 0, nullptr},
  {"Ratio", "", 2, 1, 4, 0, nullptr},
  {"Mix", "", 0.8f, 0, 1, 0, nullptr},
  {"Drive", "", 0.3f, 0, 1, 0, nullptr},
  {"Tilt", "dB", 2, -6, 6, 0, nullptr},
  {"Output", "dB", 0, -12, 12, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == OUTPUT + 1);

}  // namespace SOUND::PLUGINS::BUS
