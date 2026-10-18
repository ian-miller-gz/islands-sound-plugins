// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CYMBAL {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole TONE = 2;
constexpr Whole SPLASH = 3;
constexpr Whole LEVEL = 4;
constexpr Whole EDGE = 5;
constexpr Whole NOISE = 6;
constexpr Whole CHOKE = 7;
constexpr Whole CURVE = 8;
constexpr Whole TOUCH = 9;

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -12, 12, 0, nullptr},
  {"Decay", "s", 1.5f, 0.2f, 5, 0, nullptr},
  {"Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Splash", "", 0.5f, 0, 1, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Edge", "Hz", 2000, 1000, 10000, 0, nullptr},
  {"Noise", "", 0.15f, 0, 1, 0, nullptr},
  {"Choke", "", 0, 0, 1, 1, SWITCHES},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "", 0.5f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::CYMBAL
