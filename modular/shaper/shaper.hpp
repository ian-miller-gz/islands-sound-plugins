// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::SHAPER {

constexpr Whole CURVE = 0;
constexpr Whole DRIVE = 1;
constexpr Whole BITS = 2;
constexpr Whole RATE = 3;
constexpr Whole MIX = 4;
constexpr Whole BIAS = 5;
constexpr Whole EVEN = 6;
constexpr Whole ODD = 7;
constexpr Whole QUALITY = 8;
constexpr Whole AMOUNT = 9;
constexpr Whole PARAMETERS = 10;

constexpr Whole CV = 1;

constexpr Whole SOFT = 0;
constexpr Whole HARD = 1;
constexpr Whole TUBE = 2;
constexpr Whole FOLD = 3;
constexpr Whole CRUSH = 4;
constexpr Whole CHEBYSHEV = 5;
constexpr Whole CURVES = 6;

constexpr Whole SINGLE = 0;
constexpr Whole DOUBLE = 1;
constexpr Whole QUAD = 2;
constexpr Whole QUALITIES = 3;

constexpr Float LOUDEST = 48.0f;
constexpr Float FEWEST = 1.0f;
constexpr Float MOST = 16.0f;
constexpr Float SLOWEST = 100.0f;
constexpr Float FASTEST = 48000.0f;

inline constexpr STRING::Hot LABELS[] = {"Soft", "Hard",  "Tube",
                                         "Fold", "Crush", "Chebyshev"};
static_assert(std::size(LABELS) == CURVES);

inline constexpr STRING::Hot FACTORS[] = {"Single", "Double", "Quad"};
static_assert(std::size(FACTORS) == QUALITIES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Curve", "", SOFT, SOFT, CHEBYSHEV, CHEBYSHEV, LABELS},
  {"Drive", "dB", 12, 0, LOUDEST, 0, nullptr},
  {"Bits", "", 8, FEWEST, MOST, 0, nullptr},
  {"Rate", "Hz", 8000, SLOWEST, FASTEST, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr},
  {"Bias", "", 0, -1, 1, 0, nullptr},
  {"Even", "", 0.5f, 0, 1, 0, nullptr},
  {"Odd", "", 0.5f, 0, 1, 0, nullptr},
  {"Quality", "", DOUBLE, SINGLE, QUAD, QUAD, FACTORS},
  {"Amount", "dB", 12, -LOUDEST, LOUDEST, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::SHAPER
