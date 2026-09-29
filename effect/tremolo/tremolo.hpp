// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::TREMOLO {

constexpr Whole RATE = 0;
constexpr Whole DEPTH = 1;
constexpr Whole SHAPE = 2;
constexpr Whole PHASE = 3;
constexpr Whole PARAMETERS = 4;

inline constexpr STRING::Hot SHAPES[] = {"Sine", "Triangle", "Square"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rate", "Hz", 5, 0.1f, 20, 0, nullptr},
  {"Depth", "", 0.5f, 0, 1, 0, nullptr},
  {"Shape", "", 0, 0, 2, 2, SHAPES},
  {"Phase", "deg", 0, 0, 180, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::TREMOLO
