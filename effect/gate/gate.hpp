// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::GATE {

constexpr Whole THRESHOLD = 0;
constexpr Whole ATTACK = 1;
constexpr Whole HOLD = 2;
constexpr Whole RELEASE = 3;
constexpr Whole RANGE = 4;
constexpr Whole SIDECHAIN = 5;
constexpr Whole PARAMETERS = 6;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Threshold", "dB", -50, -80, 0, 0, nullptr},
  {"Attack", "ms", 1, 0.1f, 50, 0, nullptr},
  {"Hold", "ms", 50, 0, 500, 0, nullptr},
  {"Release", "ms", 100, 5, 2000, 0, nullptr},
  {"Range", "dB", -80, -80, 0, 0, nullptr},
  {"Sidechain", "Hz", 20, 20, 2000, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::GATE
