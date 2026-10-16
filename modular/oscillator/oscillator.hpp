// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::OSCILLATOR {

constexpr Whole WAVE = 0;
constexpr Whole WIDTH = 1;
constexpr Whole COARSE = 2;
constexpr Whole FINE = 3;
constexpr Whole GLIDE = 4;
constexpr Whole GAIN = 5;
constexpr Whole DETUNE = 6;
constexpr Whole PITCH = 7;
constexpr Whole INDEX = 8;
constexpr Whole PLAY = 9;
constexpr Whole PARAMETERS = 10;

constexpr Whole SAW = 0;
constexpr Whole PULSE = 1;
constexpr Whole TRIANGLE = 2;
constexpr Whole SINE = 3;
constexpr Whole SUPER = 4;
constexpr Whole WAVES = 5;

constexpr Whole NOTES = 0;
constexpr Whole FREE = 1;

constexpr Float SEMITONES = 24.0f;
constexpr Float CENTS = 100.0f;
constexpr Float LONGEST = 2.0f;
constexpr Float OCTAVE = 12.0f;
constexpr Float REACH = 48.0f;
constexpr Float DEEPEST = 4.0f;

inline constexpr STRING::Hot LABELS[] = {
  "Saw", "Pulse", "Triangle", "Sine", "Super"};
inline constexpr STRING::Hot PLAYS[] = {"Notes", "Free"};
static_assert(std::size(LABELS) == WAVES);
static_assert(std::size(PLAYS) == FREE + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Wave", "", SAW, SAW, SUPER, SUPER, LABELS},
  {"Width", "", 0.5f, 0.05f, 0.95f, 0, nullptr},
  {"Coarse", "st", 0, -SEMITONES, SEMITONES, Whole(SEMITONES * 2), nullptr},
  {"Fine", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"Gain", "", 0.5f, 0, 1, 0, nullptr},
  {"Detune", "", 0.5f, 0, 1, 0, nullptr},
  {"Pitch", "st", OCTAVE, -REACH, REACH, 0, nullptr},
  {"Index", "", 1, 0, DEEPEST, 0, nullptr},
  {"Play", "", NOTES, NOTES, FREE, FREE, PLAYS}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::OSCILLATOR
