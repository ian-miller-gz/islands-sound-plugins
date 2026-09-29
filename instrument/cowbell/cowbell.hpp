// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::COWBELL {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole LEVEL = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -12, 12, 0, nullptr},
  {"Decay", "s", 0.35f, 0.05f, 2, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::COWBELL
