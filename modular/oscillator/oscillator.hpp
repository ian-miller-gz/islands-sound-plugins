// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::OSCILLATOR {

constexpr Whole WAVE = 0;
constexpr Whole WIDTH = 1;
constexpr Whole COARSE = 2;
constexpr Whole FINE = 3;
constexpr Whole GLIDE = 4;
constexpr Whole GAIN = 5;
constexpr Whole PARAMETERS = 6;

constexpr Whole SAW = 0;
constexpr Whole PULSE = 1;
constexpr Whole TRIANGLE = 2;
constexpr Whole SINE = 3;
constexpr Whole SUPER = 4;
constexpr Whole WAVES = 5;

constexpr Float SEMITONES = 24.0f;
constexpr Float CENTS = 100.0f;
constexpr Float LONGEST = 2.0f;

inline constexpr STRING::Hot LABELS[] = {
  "Saw", "Pulse", "Triangle", "Sine", "Super"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == WAVES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Wave", "", SAW, SAW, SUPER, SUPER, LABELS},
  {"Width", "", 0.5f, 0.05f, 0.95f, 0, nullptr},
  {"Coarse", "st", 0, -SEMITONES, SEMITONES, Whole(SEMITONES * 2), nullptr},
  {"Fine", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"Gain", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::OSCILLATOR
