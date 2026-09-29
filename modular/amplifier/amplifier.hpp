// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::AMPLIFIER {

constexpr Whole OFFSET = 0;
constexpr Whole DEPTH = 1;
constexpr Whole GAIN = 2;
constexpr Whole PARAMETERS = 3;

constexpr Whole CV = 1;

constexpr Float INVERTED = -1.0f;
constexpr Float LOUDEST = 2.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Offset", "", 0, 0, 1, 0, nullptr},
  {"Depth", "", 1, INVERTED, 1, 0, nullptr},
  {"Gain", "", 1, 0, LOUDEST, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::AMPLIFIER
