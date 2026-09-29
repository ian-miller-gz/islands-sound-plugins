// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::WHISPER {

constexpr Whole GAIN = 0;
constexpr Whole COLOUR = 1;
constexpr Whole VOWEL = 2;
constexpr Whole MORPH = 3;
constexpr Whole TRACKING = 4;
constexpr Whole ATTACK = 5;
constexpr Whole DECAY = 6;
constexpr Whole SUSTAIN = 7;
constexpr Whole RELEASE = 8;
constexpr Whole PARAMETERS = 9;

constexpr Whole WHITE = 0;
constexpr Whole PINK = 1;
constexpr Whole BROWN = 2;
constexpr Whole BLUE = 3;
constexpr Whole COLOURS = 4;

constexpr Whole STEPS = CORE::FILTER::VOWELS - 1;
constexpr Float LAST = Float(STEPS);
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;

inline constexpr STRING::Hot NOISES[] = {"White", "Pink", "Brown", "Blue"};
static_assert(sizeof(NOISES) / sizeof(NOISES[0]) == COLOURS);

inline constexpr STRING::Hot VOWELS[] = {"A", "E", "I", "O", "U"};
static_assert(sizeof(VOWELS) / sizeof(VOWELS[0]) == CORE::FILTER::VOWELS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.5f, 0, 1, 0, nullptr},
  {"Colour", "", Float(WHITE), Float(WHITE), Float(BLUE), COLOURS - 1, NOISES},
  {"Vowel", "", 0, 0, LAST, STEPS, VOWELS},
  {"Morph", "", 0, 0, 1, 0, nullptr},
  {"Tracking", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.02f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.8f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::WHISPER
