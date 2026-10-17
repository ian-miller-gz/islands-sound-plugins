// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::SLEW {

constexpr Whole RISE = 0;
constexpr Whole FALL = 1;
constexpr Whole SHAPE = 2;
constexpr Whole AMOUNT = 3;
constexpr Whole PARAMETERS = 4;

constexpr Whole IN = 0;
constexpr Whole CV = 1;

constexpr Float QUICKEST = 0.0f;
constexpr Float SLOWEST = 5.0f;
constexpr Float BEND = 1.0f;
constexpr Float OCTAVES = 4.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rise", "s", 0.001f, QUICKEST, SLOWEST, 0, nullptr},
  {"Fall", "s", 0.001f, QUICKEST, SLOWEST, 0, nullptr},
  {"Shape", "", 0, -BEND, BEND, 0, nullptr},
  {"Amount", "oct", 1, -OCTAVES, OCTAVES, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::SLEW
