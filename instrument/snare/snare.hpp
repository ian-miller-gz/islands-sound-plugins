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
constexpr Whole BEND = 5;
constexpr Whole DROP = 6;
constexpr Whole HOLD = 7;
constexpr Whole BODY = 8;
constexpr Whole CURVE = 9;
constexpr Whole TOUCH = 10;

constexpr Float SEMITONES = 24;
constexpr Float OCTAVES = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 180, 100, 400, 0, nullptr},
  {"Tone", "Hz", 3000, 500, 10000, 0, nullptr},
  {"Snappy", "", 0.6f, 0, 1, 0, nullptr},
  {"Decay", "s", 0.25f, 0.05f, 1.5f, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Bend", "st", 4, 0, SEMITONES, 0, nullptr},
  {"Drop", "s", 0.02f, 0.005f, 0.2f, 0, nullptr},
  {"Hold", "s", 0.008f, 0, 0.05f, 0, nullptr},
  {"Body", "", 0.3f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "oct", 1, 0, OCTAVES, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::SNARE
