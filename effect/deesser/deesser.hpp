// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::DEESSER {

constexpr Whole FREQUENCY = 0;
constexpr Whole RANGE = 1;
constexpr Whole THRESHOLD = 2;
constexpr Whole RATIO = 3;
constexpr Whole ATTACK = 4;
constexpr Whole RELEASE = 5;
constexpr Whole MODE = 6;
constexpr Whole LISTEN = 7;
constexpr Whole PARAMETERS = 8;

constexpr Whole BAND = 0;
constexpr Whole WIDE = 1;
constexpr Whole MODES = 2;

constexpr Float LOWEST = 2000.0f;
constexpr Float SIBILANT = 6000.0f;
constexpr Float HIGHEST = 12000.0f;
constexpr Float DEEPEST = 24.0f;
constexpr Float DEPTH = 12.0f;
constexpr Float QUIETEST = -60.0f;
constexpr Float EDGE = -30.0f;
constexpr Float GENTLEST = 1.0f;
constexpr Float FIRM = 4.0f;
constexpr Float STEEPEST = 20.0f;
constexpr Float QUICKEST = 0.1f;
constexpr Float QUICK = 1.0f;
constexpr Float SLOW = 60.0f;
constexpr Float SLOWEST = 500.0f;

inline constexpr STRING::Hot SPANS[] = {"Band", "Wide"};
static_assert(sizeof(SPANS) / sizeof(SPANS[0]) == MODES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Frequency", "Hz", SIBILANT, LOWEST, HIGHEST, 0, nullptr},
  {"Range", "dB", DEPTH, 0, DEEPEST, 0, nullptr},
  {"Threshold", "dB", EDGE, QUIETEST, 0, 0, nullptr},
  {"Ratio", "", FIRM, GENTLEST, STEEPEST, 0, nullptr},
  {"Attack", "ms", QUICK, QUICKEST, SLOW, 0, nullptr},
  {"Release", "ms", SLOW, QUICK, SLOWEST, 0, nullptr},
  {"Mode", "", Float(BAND), Float(BAND), Float(WIDE), MODES - 1, SPANS},
  {"Listen", "", 0, 0, 1, 1, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::DEESSER
