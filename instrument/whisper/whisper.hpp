// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/glottis/glottis.hpp"
#include "../../core/table/table.hpp"
#include "whisper.words.hpp"

namespace SOUND::PLUGINS::WHISPER {

constexpr Whole GAIN = 0;
constexpr Whole COLOUR = 1;
constexpr Whole VOWEL = 2;
constexpr Whole MORPH = 3;
constexpr Whole TRACKING = 4;
constexpr Whole ATTACK = 5;
constexpr Whole DECAY = 6;
constexpr Whole SUSTAIN = 7;
constexpr Whole RELEASE = 8;
constexpr Whole VOICE = 9;
constexpr Whole SEX = 10;
constexpr Whole TARGET = 11;
constexpr Whole MOTION = 12;
constexpr Whole BREADTH = 13;
constexpr Whole VOICING = 14;
constexpr Whole MODE = 15;
constexpr Whole GLIDE = 16;
constexpr Whole WIDTH = 17;
constexpr Whole PARAMETERS = 18;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.5f, 0, 1, 0, nullptr},
  {"Colour", "", Float(WHITE), Float(WHITE), Float(BLUE), COLOURS - 1, NOISES},
  {"Vowel", "", 0, 0, LAST, STEPS, VOWELS},
  {"Morph", "", 0, 0, 1, 0, nullptr},
  {"Tracking", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.02f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.8f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Voice", "", Float(CORE::FILTER::TENOR), 0, Float(CORE::FILTER::SOPRANO),
   CORE::FILTER::SOPRANO, VOICES},
  {"Sex", "", 0, -1, 1, 0, nullptr},
  {"Target", "", 1, 0, LAST, STEPS, VOWELS},
  {"Motion", "Hz", 0.1f, STILLEST, RESTLESS, 0, nullptr},
  {"Breadth", "", 2, NARROWEST, WIDEST, 0, nullptr},
  {"Voicing", "", 0.1f, 0, 1, 0, nullptr},
  {"Mode", "", 0, 0, Float(VOICINGS), VOICINGS, MODES},
  {"Glide", "s", 0.08f, 0, TRAVEL, 0, nullptr},
  {"Width", "", 0.5f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::WHISPER
