// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::DOUBLER {

constexpr Whole VOICES = 0;
constexpr Whole DETUNE = 1;
constexpr Whole DELAY = 2;
constexpr Whole WOBBLE = 3;
constexpr Whole WIDTH = 4;
constexpr Whole MIX = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole PAIR = 0;
constexpr Whole QUARTET = 1;
constexpr Whole CHOICES = 2;

constexpr Float SUBTLE = 8.0f;
constexpr Float WIDEST = 50.0f;
constexpr Float SHORT = 15.0f;
constexpr Float LONGEST = 60.0f;
constexpr Float GENTLE = 0.3f;
constexpr Float SPREAD = 0.8f;
constexpr Float EVEN = 0.5f;

inline constexpr STRING::Hot COUNTS[] = {"2", "4"};
static_assert(sizeof(COUNTS) / sizeof(COUNTS[0]) == CHOICES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Voices", "", Float(PAIR), Float(PAIR), Float(QUARTET), CHOICES - 1, COUNTS},
  {"Detune", "ct", SUBTLE, 0, WIDEST, 0, nullptr},
  {"Delay", "ms", SHORT, 0, LONGEST, 0, nullptr},
  {"Wobble", "", GENTLE, 0, 1, 0, nullptr},
  {"Width", "", SPREAD, 0, 1, 0, nullptr},
  {"Mix", "", EVEN, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::DOUBLER
