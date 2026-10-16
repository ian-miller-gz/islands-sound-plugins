// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CLAP {

constexpr Whole SPREAD = 0;
constexpr Whole DECAY = 1;
constexpr Whole TONE = 2;
constexpr Whole LEVEL = 3;
constexpr Whole BURSTS = 4;
constexpr Whole SCATTER = 5;
constexpr Whole TAIL = 6;
constexpr Whole CUTOFF = 7;
constexpr Whole EMPHASIS = 8;
constexpr Whole CURVE = 9;
constexpr Whole TOUCH = 10;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Spread", "s", 0.011f, 0.004f, 0.03f, 0, nullptr},
  {"Decay", "s", 0.3f, 0.05f, 2, 0, nullptr},
  {"Tone", "Hz", 1100, 400, 5000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Bursts", "", 4, 1, 8, 7, nullptr},
  {"Scatter", "", 0.3f, 0, 1, 0, nullptr},
  {"Tail", "Hz", 900, 400, 5000, 0, nullptr},
  {"Cutoff", "Hz", 7000, 1000, 16000, 0, nullptr},
  {"Emphasis", "", 0.3f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "", 0.5f, 0, 2, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::CLAP
