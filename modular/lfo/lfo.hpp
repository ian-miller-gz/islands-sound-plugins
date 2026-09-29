// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::LFO {

constexpr Whole WAVE = 0;
constexpr Whole RATE = 1;
constexpr Whole DEPTH = 2;
constexpr Whole OFFSET = 3;
constexpr Whole PHASE = 4;
constexpr Whole FADE = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole SINE = 0;
constexpr Whole RANDOM = 5;

constexpr Float SLOWEST = 0.01f;
constexpr Float FASTEST = 20.0f;
constexpr Float LONGEST = 10.0f;
constexpr Float BELOW = -1.0f;
constexpr Float ABOVE = 1.0f;

inline constexpr STRING::Hot WAVES[] = {"Sine",   "Triangle", "Saw",
                                        "Square", "Hold",     "Random"};
static_assert(std::size(WAVES) == RANDOM + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Wave", "", SINE, SINE, RANDOM, RANDOM, WAVES},
  {"Rate", "Hz", 1, SLOWEST, FASTEST, 0, nullptr},
  {"Depth", "", 1, 0, 1, 0, nullptr},
  {"Offset", "", 0, BELOW, ABOVE, 0, nullptr},
  {"Phase", "", 0, 0, 1, 0, nullptr},
  {"Fade", "s", 0, 0, LONGEST, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::LFO
