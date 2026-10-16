// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/glottis/glottis.hpp"
#include "../../core/reverb/reverb.hpp"
#include "../../core/table/table.hpp"
#include "choir.words.hpp"

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
constexpr Whole VOICE = 17;
constexpr Whole TARGET = 18;
constexpr Whole TENSION = 19;
constexpr Whole DELAY = 20;
constexpr Whole DRIFT = 21;
constexpr Whole SINGERS = 22;
constexpr Whole SIZE = 23;
constexpr Whole TAIL = 24;
constexpr Whole MODE = 25;
constexpr Whole GLIDE = 26;
constexpr Whole PARAMETERS = 27;

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
  {"Release", "s", 0.8f, QUICKEST, SLOWEST, 0, nullptr},
  {"Voice", "", Float(MIXED), 0, Float(MIXED), MIXED, VOICES},
  {"Target", "", 1, 0, LAST, STEPS, VOWELS},
  {"Tension", "", 0.5f, 0, 1, 0, nullptr},
  {"Delay", "s", 0.3f, 0, SLOWEST, 0, nullptr},
  {"Drift", "ct", 4.0f, 0, WANDER, 0, nullptr},
  {"Singers", "", Float(SEATS), Float(FEWEST), Float(SEATS), SECTIONS, nullptr},
  {"Size", "", 0.8f, CORE::REVERB::SMALLEST, CORE::REVERB::LARGEST, 0, nullptr},
  {"Tail", "s", 1.4f, BRIEFEST, LONGEST, 0, nullptr},
  {"Mode", "", 0, 0, Float(VOICINGS), VOICINGS, MODES},
  {"Glide", "s", 0.08f, 0, TRAVEL, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::CHOIR
