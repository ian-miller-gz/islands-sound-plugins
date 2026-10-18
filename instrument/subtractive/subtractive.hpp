// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::SUBTRACTIVE {

constexpr Whole VOICES = 8;
constexpr Whole OSCILLATORS = 2;

constexpr Whole GAIN = 0;
constexpr Whole SHAPE = 1;
constexpr Whole SPREAD = 2;
constexpr Whole CUTOFF = 3;
constexpr Whole RESONANCE = 4;
constexpr Whole DEPTH = 5;
constexpr Whole SWEEP = 6;
constexpr Whole CLOSE = 7;
constexpr Whole ATTACK = 8;
constexpr Whole DECAY = 9;
constexpr Whole SUSTAIN = 10;
constexpr Whole RELEASE = 11;
constexpr Whole MODE = 12;
constexpr Whole KEYBOARD = 13;
constexpr Whole ASSIGN = 14;
constexpr Whole GLIDE = 15;
constexpr Whole PARAMETERS = 16;

constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.001f;
constexpr Float CENTS = 25;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 12000;
constexpr Float PEAK = 0.95f;
constexpr Float REACH = 8000;
constexpr Float LONGEST = 3;

inline constexpr STRING::Hot SHAPES[] = {"Saw", "Pulse", "Triangle"};
inline constexpr STRING::Hot MODES[] = {"Low", "Band", "High", "Notch"};
inline constexpr STRING::Hot ASSIGNS[] = {"Poly", "Mono"};

constexpr Whole WAVES = std::size(SHAPES) - 1;
constexpr Whole KINDS = std::size(MODES) - 1;
constexpr Whole WAYS = std::size(ASSIGNS) - 1;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.30f, 0, 1, 0, nullptr},
  {"Shape", "", 0, 0, WAVES, WAVES, SHAPES},
  {"Spread", "ct", 7, 0, CENTS, 0, nullptr},
  {"Cutoff", "Hz", 1200, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.30f, 0, PEAK, 0, nullptr},
  {"Depth", "Hz", 2400, 0, REACH, 0, nullptr},
  {"Sweep", "s", 0.011f, QUICKEST, SLOWEST, 0, nullptr},
  {"Close", "s", 0.401f, QUICKEST, SLOWEST, 0, nullptr},
  {"Attack", "s", 0.011f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.1946f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.70f, 0, 1, 0, nullptr},
  {"Release", "s", 0.251f, QUICKEST, SLOWEST, 0, nullptr},
  {"Mode", "", 0, 0, KINDS, KINDS, MODES},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Assign", "", 0, 0, WAYS, WAYS, ASSIGNS},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::SUBTRACTIVE
