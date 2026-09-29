// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::CONGA {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole BEND = 2;
constexpr Whole LEVEL = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 190, 100, 400, 0, nullptr},
  {"Decay", "s", 0.3f, 0.05f, 1.5f, 0, nullptr},
  {"Bend", "", 0.2f, 0, 1, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::CONGA
