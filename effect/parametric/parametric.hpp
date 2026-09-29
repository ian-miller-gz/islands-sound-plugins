// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PARAMETRIC {

constexpr Whole BANDS = 4;
constexpr Whole GAIN = 0;
constexpr Whole FREQUENCY = GAIN + BANDS;
constexpr Whole Q = FREQUENCY + BANDS;
constexpr Whole PARAMETERS = Q + BANDS;

constexpr Float CUT = -18;
constexpr Float BOOST = 18;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float NARROW = 10;
constexpr Float BROAD = 0.1f;
constexpr Float FLAT = 0.707f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain1", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Gain2", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Gain3", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Gain4", "dB", 0, CUT, BOOST, 0, nullptr},
  {"Frequency1", "Hz", 100, LOWEST, HIGHEST, 0, nullptr},
  {"Frequency2", "Hz", 500, LOWEST, HIGHEST, 0, nullptr},
  {"Frequency3", "Hz", 2000, LOWEST, HIGHEST, 0, nullptr},
  {"Frequency4", "Hz", 8000, LOWEST, HIGHEST, 0, nullptr},
  {"Q1", "", FLAT, BROAD, NARROW, 0, nullptr},
  {"Q2", "", 1, BROAD, NARROW, 0, nullptr},
  {"Q3", "", 1, BROAD, NARROW, 0, nullptr},
  {"Q4", "", FLAT, BROAD, NARROW, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PARAMETRIC
