// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::EXPANDER {

constexpr Whole THRESHOLD = 0;
constexpr Whole RATIO = 1;
constexpr Whole ATTACK = 2;
constexpr Whole RELEASE = 3;
constexpr Whole KNEE = 4;
constexpr Whole PARAMETERS = 5;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Threshold", "dB", -40, -80, 0, 0, nullptr},
  {"Ratio", "", 2, 1, 10, 0, nullptr},
  {"Attack", "ms", 5, 0.1f, 100, 0, nullptr},
  {"Release", "ms", 100, 5, 2000, 0, nullptr},
  {"Knee", "dB", 6, 0, 24, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::EXPANDER
