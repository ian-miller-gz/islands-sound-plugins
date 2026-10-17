// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/glottis/glottis.hpp"
#include "../../core/table/table.hpp"
#include "vowel.words.hpp"

namespace SOUND::PLUGINS::VOWEL {

constexpr Whole GAIN = 0;
constexpr Whole VOWEL = 1;
constexpr Whole MORPH = 2;
constexpr Whole SEX = 3;
constexpr Whole TENSION = 4;
constexpr Whole BREATH = 5;
constexpr Whole RATE = 6;
constexpr Whole DEPTH = 7;
constexpr Whole DELAY = 8;
constexpr Whole ATTACK = 9;
constexpr Whole DECAY = 10;
constexpr Whole SUSTAIN = 11;
constexpr Whole RELEASE = 12;
constexpr Whole VOICE = 13;
constexpr Whole TARGET = 14;
constexpr Whole MOTION = 15;
constexpr Whole EFFORT = 16;
constexpr Whole DRIFT = 17;
constexpr Whole MODE = 18;
constexpr Whole GLIDE = 19;
constexpr Whole POLYPHONY = 20;
constexpr Whole WIDTH = 21;
constexpr Whole PARAMETERS = 22;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.5f, 0, 1, 0, nullptr},
  {"Vowel", "", Float(CORE::FILTER::A), 0, last(VOWELS), steps(VOWELS), VOWELS},
  {"Morph", "", 0, 0, 1, 0, nullptr},
  {"Sex", "", 0, -1, 1, 0, nullptr},
  {"Tension", "", 0.5f, 0, 1, 0, nullptr},
  {"Breath", "", 0.1f, 0, 1, 0, nullptr},
  {"Rate", "Hz", 5.5f, VIBRATO::SLOWEST, VIBRATO::FASTEST, 0, nullptr},
  {"Depth", "ct", 25.0f, 0, VIBRATO::WIDEST, 0, nullptr},
  {"Delay", "s", 0.4f, 0, SLOWEST, 0, nullptr},
  {"Attack", "s", 0.05f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.8f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Voice", "", Float(CORE::FILTER::TENOR), 0, last(VOICES), steps(VOICES),
   VOICES},
  {"Target", "", Float(CORE::FILTER::E), 0, last(VOWELS), steps(VOWELS),
   VOWELS},
  {"Motion", "Hz", 0.1f, SWAY::SLOWEST, SWAY::FASTEST, 0, nullptr},
  {"Effort", "", 0.5f, 0, 1, 0, nullptr},
  {"Drift", "ct", 4.0f, 0, WANDER, 0, nullptr},
  {"Mode", "", 0, 0, last(MODES), steps(MODES), MODES},
  {"Glide", "s", 0.08f, 0, LONGEST, 0, nullptr},
  {"Voices", "", Float(THROATS), 1, Float(THROATS), THROATS - 1, nullptr},
  {"Width", "", 0.3f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::VOWEL
