// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::KICK {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole ATTACK = 2;
constexpr Whole DRIVE = 3;
constexpr Whole TONE = 4;
constexpr Whole LEVEL = 5;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 55, 30, 150, 0, nullptr},
  {"Decay", "s", 0.5f, 0.05f, 3, 0, nullptr},
  {"Attack", "", 0.4f, 0, 1, 0, nullptr},
  {"Drive", "", 0.2f, 0, 1, 0, nullptr},
  {"Tone", "Hz", 4000, 200, 12000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::KICK
