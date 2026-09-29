// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::ARPEGGIATOR {

constexpr Whole MODE = 0;
constexpr Whole TEMPO = 1;
constexpr Whole DIVISION = 2;
constexpr Whole OCTAVES = 3;
constexpr Whole GATE = 4;
constexpr Whole SEED = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole MODES = 4;
constexpr Whole DIVISIONS = 7;
constexpr Whole SIXTEENTH = 4;
constexpr Whole RANGE = 4;
constexpr Whole SEEDS = 999;
constexpr Float SLOWEST = 20.0f;
constexpr Float FASTEST = 300.0f;
constexpr Float SHORTEST = 0.05f;

inline constexpr STRING::Hot SHAPES[] = {"Up", "Down", "Updown", "Random"};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == MODES);

inline constexpr STRING::Hot LABELS[] = {"1/2",  "1/4",   "1/8", "1/8T",
                                         "1/16", "1/16T", "1/32"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == DIVISIONS);

inline constexpr Float BEATS[] = {0.5f, 1, 2, 3, 4, 6, 8};
static_assert(sizeof(BEATS) / sizeof(BEATS[0]) == DIVISIONS);

inline constexpr STRING::Hot SPANS[] = {"1", "2", "3", "4"};
static_assert(sizeof(SPANS) / sizeof(SPANS[0]) == RANGE);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Mode", "", 0, 0, MODES - 1, MODES - 1, SHAPES},
  {"Tempo", "bpm", 120, SLOWEST, FASTEST, 0, nullptr},
  {"Division", "", SIXTEENTH, 0, DIVISIONS - 1, DIVISIONS - 1, LABELS},
  {"Octaves", "", 1, 1, RANGE, RANGE - 1, SPANS},
  {"Gate", "", 0.5f, SHORTEST, 1, 0, nullptr},
  {"Seed", "", 0, 0, SEEDS, SEEDS, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::ARPEGGIATOR
