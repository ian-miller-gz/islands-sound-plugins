// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/glottis/glottis.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::VOWEL {

constexpr Whole GAIN = 0;
constexpr Whole VOWEL = 1;
constexpr Whole MORPH = 2;
constexpr Whole SEX = 3;
constexpr Whole OPEN = 4;
constexpr Whole BREATH = 5;
constexpr Whole RATE = 6;
constexpr Whole DEPTH = 7;
constexpr Whole DELAY = 8;
constexpr Whole ATTACK = 9;
constexpr Whole DECAY = 10;
constexpr Whole SUSTAIN = 11;
constexpr Whole RELEASE = 12;
constexpr Whole PARAMETERS = 13;

constexpr Whole STEPS = CORE::FILTER::VOWELS - 1;
constexpr Float LAST = Float(STEPS);
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;

namespace VIBRATO {
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 12.0f;
constexpr Float WIDEST = 100.0f;
}  // namespace VIBRATO

inline constexpr STRING::Hot VOWELS[] = {"A", "E", "I", "O", "U"};
static_assert(sizeof(VOWELS) / sizeof(VOWELS[0]) == CORE::FILTER::VOWELS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.5f, 0, 1, 0, nullptr},
  {"Vowel", "", 0, 0, LAST, STEPS, VOWELS},
  {"Morph", "", 0, 0, 1, 0, nullptr},
  {"Sex", "", 0, -1, 1, 0, nullptr},
  {"Open", "", 0.6f, CORE::GLOTTIS::CLOSEST, CORE::GLOTTIS::WIDEST, 0, nullptr},
  {"Breath", "", 0.1f, 0, 1, 0, nullptr},
  {"Rate", "Hz", 5.5f, VIBRATO::SLOWEST, VIBRATO::FASTEST, 0, nullptr},
  {"Depth", "ct", 25.0f, 0, VIBRATO::WIDEST, 0, nullptr},
  {"Delay", "s", 0.4f, 0, SLOWEST, 0, nullptr},
  {"Attack", "s", 0.05f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.8f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::VOWEL
