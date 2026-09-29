// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::SNARE {

constexpr Whole TUNE = 0;
constexpr Whole TONE = 1;
constexpr Whole SNAPPY = 2;
constexpr Whole DECAY = 3;
constexpr Whole LEVEL = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 180, 100, 400, 0, nullptr},
  {"Tone", "Hz", 3000, 500, 10000, 0, nullptr},
  {"Snappy", "", 0.6f, 0, 1, 0, nullptr},
  {"Decay", "s", 0.25f, 0.05f, 1.5f, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::SNARE
