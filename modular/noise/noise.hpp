// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::NOISE {

constexpr Whole COLOUR = 0;
constexpr Whole GAIN = 1;
constexpr Whole GATE = 2;
constexpr Whole MODE = 3;
constexpr Whole RATE = 4;
constexpr Whole AMOUNT = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole WHITE = 0;
constexpr Whole PINK = 1;
constexpr Whole BROWN = 2;
constexpr Whole BLUE = 3;
constexpr Whole VIOLET = 4;
constexpr Whole VELVET = 5;
constexpr Whole COLOURS = 6;

constexpr Whole PASS = 0;
constexpr Whole HOLD = 1;
constexpr Whole WALK = 2;

constexpr Whole CV = 0;

constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 20000.0f;
constexpr Float OCTAVES = 4.0f;

inline constexpr STRING::Hot LABELS[] = {"White", "Pink",   "Brown",
                                         "Blue",  "Violet", "Velvet"};
static_assert(std::size(LABELS) == COLOURS);

inline constexpr STRING::Hot MODES[] = {"Noise", "Hold", "Walk"};
static_assert(std::size(MODES) == WALK + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Colour", "", WHITE, WHITE, VELVET, VELVET, LABELS},
  {"Gain", "", 0.25f, 0, 1, 0, nullptr},
  {"Gate", "", 1, 0, 1, 1, nullptr},
  {"Mode", "", PASS, PASS, WALK, WALK, MODES},
  {"Rate", "Hz", 400, SLOWEST, FASTEST, 0, nullptr},
  {"Amount", "oct", 1, -OCTAVES, OCTAVES, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::NOISE
