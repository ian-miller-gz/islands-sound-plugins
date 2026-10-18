// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CLAVE {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole LEVEL = 2;
constexpr Whole TONE = 3;
constexpr Whole RIM = 4;
constexpr Whole RING = 5;
constexpr Whole CURVE = 6;
constexpr Whole TOUCH = 7;
constexpr Whole KEY = 8;

constexpr Float SEMITONES = 12;

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 2500, 1000, 5000, 0, nullptr},
  {"Decay", "s", 0.06f, 0.01f, 0.5f, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Tone", "Hz", 300, 100, 3000, 0, nullptr},
  {"Rim", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Ring", "", 0.25f, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "", 0.5f, 0, 1, 0, nullptr},
  {"Key", "", 0, 0, 1, 1, SWITCHES}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::CLAVE
