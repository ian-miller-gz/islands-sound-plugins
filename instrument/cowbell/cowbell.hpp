// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::COWBELL {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole LEVEL = 2;
constexpr Whole TONE = 3;
constexpr Whole INTERVAL = 4;
constexpr Whole CLANK = 5;
constexpr Whole CURVE = 6;
constexpr Whole TOUCH = 7;
constexpr Whole KEY = 8;

constexpr Float SEMITONES = 12;

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Decay", "s", 0.35f, 0.05f, 2, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Tone", "Hz", 2640, 800, 6000, 0, nullptr},
  {"Interval", "st", 6.8f, 3, SEMITONES, 0, nullptr},
  {"Clank", "", 0.6f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "", 0.5f, 0, 1, 0, nullptr},
  {"Key", "", 0, 0, 1, 1, SWITCHES}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::COWBELL
