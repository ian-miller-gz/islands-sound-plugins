// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::AUTOPAN {

constexpr Whole RATE = 0;
constexpr Whole DEPTH = 1;
constexpr Whole SHAPE = 2;
constexpr Whole SYNC = 3;
constexpr Whole TEMPO = 4;
constexpr Whole DIVISION = 5;
constexpr Whole PARAMETERS = 6;

inline constexpr STRING::Hot SHAPES[] = {"Sine", "Triangle", "Square"};
inline constexpr STRING::Hot DIVISIONS[] = {"1/1", "1/2", "1/4", "1/8", "1/16"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rate", "Hz", 1, 0.05f, 20, 0, nullptr},
  {"Depth", "", 0.8f, 0, 1, 0, nullptr},
  {"Shape", "", 0, 0, 2, 2, SHAPES},
  {"Sync", "", 0, 0, 1, 1, nullptr},
  {"Tempo", "bpm", 120, 40, 240, 0, nullptr},
  {"Division", "", 2, 0, 4, 4, DIVISIONS}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::AUTOPAN
