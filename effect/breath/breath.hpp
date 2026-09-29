// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::BREATH {

constexpr Whole THRESHOLD = 0;
constexpr Whole REDUCTION = 1;
constexpr Whole FREQUENCY = 2;
constexpr Whole ATTACK = 3;
constexpr Whole RELEASE = 4;
constexpr Whole PARAMETERS = 5;

constexpr Float QUIETEST = -80.0f;
constexpr Float FLOOR = -40.0f;
constexpr Float DEEPEST = 40.0f;
constexpr Float DEPTH = 12.0f;
constexpr Float LOWEST = 1000.0f;
constexpr Float AIRY = 3000.0f;
constexpr Float HIGHEST = 10000.0f;
constexpr Float QUICKEST = 0.1f;
constexpr Float QUICK = 2.0f;
constexpr Float BRISK = 5.0f;
constexpr Float SLOW = 80.0f;
constexpr Float SLOWEST = 500.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Threshold", "dB", FLOOR, QUIETEST, 0, 0, nullptr},
  {"Reduction", "dB", DEPTH, 0, DEEPEST, 0, nullptr},
  {"Frequency", "Hz", AIRY, LOWEST, HIGHEST, 0, nullptr},
  {"Attack", "ms", QUICK, QUICKEST, SLOW, 0, nullptr},
  {"Release", "ms", SLOW, BRISK, SLOWEST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::BREATH
