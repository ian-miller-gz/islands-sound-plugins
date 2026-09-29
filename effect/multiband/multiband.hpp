// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::MULTIBAND {

constexpr Whole BANDS = 3;
constexpr Whole CROSSOVERS = BANDS - 1;
constexpr Whole CROSSOVER = 0;
constexpr Whole THRESHOLD = CROSSOVER + CROSSOVERS;
constexpr Whole RATIO = THRESHOLD + BANDS;
constexpr Whole ATTACK = RATIO + BANDS;
constexpr Whole RELEASE = ATTACK + BANDS;
constexpr Whole GAIN = RELEASE + BANDS;
constexpr Whole PARAMETERS = GAIN + BANDS;

constexpr Float QUIETEST = -60;
constexpr Float FULL = 0;
constexpr Float RESTING = -18;
constexpr Float GENTLE = 3;
constexpr Float STEEPEST = 20;
constexpr Float QUICKEST = 0.1f;
constexpr Float SLOWEST = 2000;
constexpr Float LOUDEST = 24;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Crossover1", "Hz", 200, 40, 1000, 0, nullptr},
  {"Crossover2", "Hz", 2500, 1000, 12000, 0, nullptr},
  {"Threshold1", "dB", RESTING, QUIETEST, FULL, 0, nullptr},
  {"Threshold2", "dB", RESTING, QUIETEST, FULL, 0, nullptr},
  {"Threshold3", "dB", RESTING, QUIETEST, FULL, 0, nullptr},
  {"Ratio1", "", GENTLE, 1, STEEPEST, 0, nullptr},
  {"Ratio2", "", GENTLE, 1, STEEPEST, 0, nullptr},
  {"Ratio3", "", GENTLE, 1, STEEPEST, 0, nullptr},
  {"Attack1", "ms", 20, QUICKEST, SLOWEST, 0, nullptr},
  {"Attack2", "ms", 10, QUICKEST, SLOWEST, 0, nullptr},
  {"Attack3", "ms", 5, QUICKEST, SLOWEST, 0, nullptr},
  {"Release1", "ms", 200, QUICKEST, SLOWEST, 0, nullptr},
  {"Release2", "ms", 150, QUICKEST, SLOWEST, 0, nullptr},
  {"Release3", "ms", 100, QUICKEST, SLOWEST, 0, nullptr},
  {"Gain1", "dB", FULL, FULL, LOUDEST, 0, nullptr},
  {"Gain2", "dB", FULL, FULL, LOUDEST, 0, nullptr},
  {"Gain3", "dB", FULL, FULL, LOUDEST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::MULTIBAND
