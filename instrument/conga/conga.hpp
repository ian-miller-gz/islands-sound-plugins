// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CONGA {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole BEND = 2;
constexpr Whole LEVEL = 3;
constexpr Whole DROP = 4;
constexpr Whole SLOPE = 5;
constexpr Whole TONE = 6;
constexpr Whole RING = 7;
constexpr Whole CURVE = 8;
constexpr Whole KEY = 9;

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 190, 100, 400, 0, nullptr},
  {"Decay", "s", 0.3f, 0.05f, 1.5f, 0, nullptr},
  {"Bend", "", 0.2f, 0, 1, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Drop", "s", 0.03f, 0.005f, 0.3f, 0, nullptr},
  {"Slope", "", 0, -1, 1, 0, nullptr},
  {"Tone", "Hz", 1500, 500, 6000, 0, nullptr},
  {"Ring", "", 0.25f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Key", "", 0, 0, 1, 1, SWITCHES}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::CONGA
