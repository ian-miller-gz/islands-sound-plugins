// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::TINE {

constexpr Whole TONE = 0;
constexpr Whole VELOCITY = 1;
constexpr Whole STRIKE = 2;
constexpr Whole COUPLING = 3;
constexpr Whole BELL = 4;
constexpr Whole RING = 5;
constexpr Whole PICKUP = 6;
constexpr Whole VOICING = 7;
constexpr Whole DECAY = 8;
constexpr Whole RELEASE = 9;
constexpr Whole RATE = 10;
constexpr Whole DEPTH = 11;
constexpr Whole STEREO = 12;
constexpr Whole TUNE = 13;
constexpr Whole BEND = 14;
constexpr Whole VOLUME = 15;
constexpr Whole PARAMETERS = 16;

constexpr Float INDEX = 4;
constexpr Float CENTS = 100;
constexpr Float SEMITONES = 12;
constexpr Float QUICKEST = 0.01f;
constexpr Float SHORTEST = 0.02f;
constexpr Float BRIEF = 2;
constexpr Float BRIEFEST = 0.5f;
constexpr Float LONGEST = 20;
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 12;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tone", "", 1.2f, 0, INDEX, 0, nullptr},
  {"Velocity", "", 0.8f, 0, 1, 0, nullptr},
  {"Strike", "s", 0.3f, QUICKEST, BRIEF, 0, nullptr},
  {"Coupling", "", 0.2f, 0, 1, 0, nullptr},
  {"Bell", "", 0.25f, 0, 1, 0, nullptr},
  {"Ring", "s", 0.4f, QUICKEST, BRIEF, 0, nullptr},
  {"Pickup", "", 0.5f, 0, 1, 0, nullptr},
  {"Voicing", "", 0.3f, -1, 1, 0, nullptr},
  {"Decay", "s", 6, BRIEFEST, LONGEST, 0, nullptr},
  {"Release", "s", 0.12f, SHORTEST, BRIEF, 0, nullptr},
  {"Rate", "Hz", 4.5f, SLOWEST, FASTEST, 0, nullptr},
  {"Depth", "", 0.3f, 0, 1, 0, nullptr},
  {"Stereo", "", 1, 0, 1, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::TINE
