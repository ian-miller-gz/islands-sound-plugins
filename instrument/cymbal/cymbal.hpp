// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::CYMBAL {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole TONE = 2;
constexpr Whole SPLASH = 3;
constexpr Whole LEVEL = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -12, 12, 0, nullptr},
  {"Decay", "s", 1.5f, 0.2f, 5, 0, nullptr},
  {"Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Splash", "", 0.5f, 0, 1, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::CYMBAL
