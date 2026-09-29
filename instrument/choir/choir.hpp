// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/glottis/glottis.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CHOIR {

constexpr Whole GAIN = 0;
constexpr Whole VOWEL = 1;
constexpr Whole MORPH = 2;
constexpr Whole MOTION = 3;
constexpr Whole SEX = 4;
constexpr Whole SPREAD = 5;
constexpr Whole DETUNE = 6;
constexpr Whole SCATTER = 7;
constexpr Whole BREATH = 8;
constexpr Whole RATE = 9;
constexpr Whole DEPTH = 10;
constexpr Whole ENSEMBLE = 11;
constexpr Whole ROOM = 12;
constexpr Whole ATTACK = 13;
constexpr Whole DECAY = 14;
constexpr Whole SUSTAIN = 15;
constexpr Whole RELEASE = 16;
constexpr Whole PARAMETERS = 17;

constexpr Whole STEPS = CORE::FILTER::VOWELS - 1;
constexpr Float LAST = Float(STEPS);
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;
constexpr Float STILLEST = 0.01f;
constexpr Float RESTLESS = 2.0f;
constexpr Float WIDEST = 50.0f;
constexpr Float LATEST = 0.25f;

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
  {"Morph", "", 0.3f, 0, 1, 0, nullptr},
  {"Motion", "Hz", 0.1f, STILLEST, RESTLESS, 0, nullptr},
  {"Sex", "", 0, -1, 1, 0, nullptr},
  {"Spread", "", 0.5f, 0, 1, 0, nullptr},
  {"Detune", "ct", 12.0f, 0, WIDEST, 0, nullptr},
  {"Scatter", "s", 0.04f, 0, LATEST, 0, nullptr},
  {"Breath", "", 0.2f, 0, 1, 0, nullptr},
  {"Rate", "Hz", 5.0f, VIBRATO::SLOWEST, VIBRATO::FASTEST, 0, nullptr},
  {"Depth", "ct", 20.0f, 0, VIBRATO::WIDEST, 0, nullptr},
  {"Ensemble", "", 0.5f, 0, 1, 0, nullptr},
  {"Room", "", 0.25f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.5f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.9f, 0, 1, 0, nullptr},
  {"Release", "s", 0.8f, QUICKEST, SLOWEST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::CHOIR
