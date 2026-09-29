// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUCK {

constexpr Whole TUNE = 0;
constexpr Whole TONE = 1;
constexpr Whole POSITION = 2;
constexpr Whole DAMPING = 3;
constexpr Whole DECAY = 4;
constexpr Whole RELEASE = 5;
constexpr Whole BODY = 6;
constexpr Whole SIZE = 7;
constexpr Whole VOLUME = 8;
constexpr Whole PARAMETERS = 9;

constexpr Float CENTS = 200;
constexpr Float DULLEST = 200;
constexpr Float BRIGHTEST = 20000;
constexpr Float NEAREST = 0.02f;
constexpr Float MIDDLE = 0.5f;
constexpr Float SHORTEST = 0.01f;
constexpr Float LONGEST = 20;
constexpr Float SMALLEST = 0.5f;
constexpr Float LARGEST = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Tone", "Hz", 5000, DULLEST, BRIGHTEST, 0, nullptr},
  {"Position", "", 0.13f, NEAREST, MIDDLE, 0, nullptr},
  {"Damping", "", 0.4f, 0, 1, 0, nullptr},
  {"Decay", "s", 3, SHORTEST, LONGEST, 0, nullptr},
  {"Release", "s", 0.3f, SHORTEST, LONGEST, 0, nullptr},
  {"Body", "", 0.5f, 0, 1, 0, nullptr},
  {"Size", "", 1, SMALLEST, LARGEST, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUCK
