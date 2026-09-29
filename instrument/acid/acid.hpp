// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::ACID {

constexpr Whole TUNE = 0;
constexpr Whole WAVE = 1;
constexpr Whole CUTOFF = 2;
constexpr Whole RESONANCE = 3;
constexpr Whole ENVELOPE = 4;
constexpr Whole DECAY = 5;
constexpr Whole ACCENT = 6;
constexpr Whole SLIDE = 7;
constexpr Whole VOLUME = 8;
constexpr Whole PARAMETERS = 9;

constexpr Float SEMITONES = 12;
constexpr Float OCTAVES = 5;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float SHORTEST = 0.2f;
constexpr Float LONGEST = 2;
constexpr Float QUICKEST = 0.01f;
constexpr Float SLOWEST = 0.5f;

inline constexpr STRING::Hot WAVES[] = {"Saw", "Square"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Wave", "", 0, 0, 1, 1, WAVES},
  {"Cutoff", "Hz", 500, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.6f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Decay", "s", 0.5f, SHORTEST, LONGEST, 0, nullptr},
  {"Accent", "", 0.5f, 0, 1, 0, nullptr},
  {"Slide", "s", 0.06f, QUICKEST, SLOWEST, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::ACID
