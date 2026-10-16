// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::TOM {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole BEND = 2;
constexpr Whole TONE = 3;
constexpr Whole LEVEL = 4;
constexpr Whole DROP = 5;
constexpr Whole SLOPE = 6;
constexpr Whole RING = 7;
constexpr Whole CURVE = 8;
constexpr Whole TOUCH = 9;
constexpr Whole KEY = 10;

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 90, 40, 250, 0, nullptr},
  {"Decay", "s", 0.4f, 0.05f, 2, 0, nullptr},
  {"Bend", "", 0.3f, 0, 1, 0, nullptr},
  {"Tone", "Hz", 4000, 500, 12000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Drop", "s", 0.2f, 0.01f, 1, 0, nullptr},
  {"Slope", "", 0, -1, 1, 0, nullptr},
  {"Ring", "", 0.25f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "oct", 1, 0, 3, 0, nullptr},
  {"Key", "", 0, 0, 1, 1, SWITCHES}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::TOM
