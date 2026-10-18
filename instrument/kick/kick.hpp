// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::KICK {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole ATTACK = 2;
constexpr Whole DRIVE = 3;
constexpr Whole TONE = 4;
constexpr Whole LEVEL = 5;
constexpr Whole BEND = 6;
constexpr Whole DROP = 7;
constexpr Whole SLOPE = 8;
constexpr Whole CLICK = 9;
constexpr Whole SIGH = 10;
constexpr Whole LOW = 11;
constexpr Whole HIGH = 12;
constexpr Whole CURVE = 13;
constexpr Whole TOUCH = 14;
constexpr Whole KEY = 15;

inline constexpr STRING::Hot CLICKS[] = {"Noise", "Pulse"};
inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 55, 30, 150, 0, nullptr},
  {"Decay", "s", 0.5f, 0.05f, 3, 0, nullptr},
  {"Attack", "", 0.4f, 0, 1, 0, nullptr},
  {"Drive", "", 0.2f, 0, 1, 0, nullptr},
  {"Tone", "Hz", 4000, 200, 12000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Bend", "st", 16, 0, 36, 0, nullptr},
  {"Drop", "s", 0.03f, 0.005f, 0.3f, 0, nullptr},
  {"Slope", "", 0, -1, 1, 0, nullptr},
  {"Click", "", 0, 0, 1, 1, CLICKS},
  {"Sigh", "", 0.3f, 0, 1, 0, nullptr},
  {"Low", "dB", 0, -12, 12, 0, nullptr},
  {"High", "dB", 0, -12, 12, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "oct", 1, 0, 3, 0, nullptr},
  {"Key", "", 0, 0, 1, 1, SWITCHES}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::KICK
