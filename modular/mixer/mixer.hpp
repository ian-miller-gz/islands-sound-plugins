// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::MIXER {

constexpr Whole INS = 4;
constexpr Whole LEVEL = 0;
constexpr Whole PAN = LEVEL + INS;
constexpr Whole GAIN = PAN + INS;
constexpr Whole PARAMETERS = GAIN + 1;

constexpr Float LEFT = -1;
constexpr Float RIGHT = 1;
constexpr Float LOUDEST = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Level1", "", 1, 0, 1, 0, nullptr},
  {"Level2", "", 1, 0, 1, 0, nullptr},
  {"Level3", "", 1, 0, 1, 0, nullptr},
  {"Level4", "", 1, 0, 1, 0, nullptr},
  {"Pan1", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan2", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan3", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan4", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Gain", "", 1, 0, LOUDEST, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::MIXER
