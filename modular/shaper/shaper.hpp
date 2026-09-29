// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::SHAPER {

constexpr Whole CURVE = 0;
constexpr Whole DRIVE = 1;
constexpr Whole BITS = 2;
constexpr Whole RATE = 3;
constexpr Whole MIX = 4;
constexpr Whole PARAMETERS = 5;

constexpr Whole SOFT = 0;
constexpr Whole HARD = 1;
constexpr Whole TUBE = 2;
constexpr Whole FOLD = 3;
constexpr Whole CRUSH = 4;
constexpr Whole CURVES = 5;

constexpr Float LOUDEST = 48.0f;
constexpr Float FEWEST = 1.0f;
constexpr Float MOST = 16.0f;
constexpr Float SLOWEST = 100.0f;
constexpr Float FASTEST = 48000.0f;

inline constexpr STRING::Hot LABELS[] = {
  "Soft", "Hard", "Tube", "Fold", "Crush"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == CURVES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Curve", "", SOFT, SOFT, CRUSH, CRUSH, LABELS},
  {"Drive", "dB", 12, 0, LOUDEST, 0, nullptr},
  {"Bits", "", 8, FEWEST, MOST, 0, nullptr},
  {"Rate", "Hz", 8000, SLOWEST, FASTEST, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::SHAPER
