// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "ladder.indices.hpp"

namespace SOUND::LADDER {

constexpr Whole OSCILLATORS = 3;
constexpr Whole FEET = 5;
constexpr Whole POSITIONS = 6;
constexpr Whole TRACKS = 3;
constexpr Float CENTS = 200;
constexpr Float SEMITONES = 7;
constexpr Float OCTAVES = 5;
constexpr Float OVERLOAD = 4;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10;
constexpr Float LONGEST = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;

inline constexpr STRING::Hot RANGES[] = {"Lo", "32", "16", "8", "4", "2"};
inline constexpr STRING::Hot WAVES[] = {"Triangle", "Sharktooth", "Saw",
                                        "Square",   "Wide",       "Narrow"};
inline constexpr STRING::Hot REVERSED[] = {"Triangle", "Sharktooth", "Reverse",
                                           "Saw",      "Square",     "Wide"};
inline constexpr STRING::Hot COLOURS[] = {"White", "Pink"};
inline constexpr STRING::Hot FRACTIONS[] = {"Off", "1/3", "2/3", "Full"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"Modulation", "", 0, 0, 1, 0, nullptr},
  {"Mix", "", 0, 0, 1, 0, nullptr},
  {"Pitch", "", 1, 0, 1, 1, nullptr},
  {"Filter", "", 0, 0, 1, 1, nullptr},
  {"Range1", "", 3, 0, FEET, FEET, RANGES},
  {"Wave1", "", 2, 0, FEET, FEET, WAVES},
  {"Level1", "", 0.7f, 0, 1, 0, nullptr},
  {"Range2", "", 3, 0, FEET, FEET, RANGES},
  {"Wave2", "", 2, 0, FEET, FEET, WAVES},
  {"Frequency2", "st", 0.1f, -SEMITONES, SEMITONES, 0, nullptr},
  {"Level2", "", 0.7f, 0, 1, 0, nullptr},
  {"Range3", "", 0, 0, FEET, FEET, RANGES},
  {"Wave3", "", 0, 0, FEET, FEET, REVERSED},
  {"Frequency3", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Level3", "", 0, 0, 1, 0, nullptr},
  {"Keyboard", "", 0, 0, 1, 1, nullptr},
  {"Noise", "", 0, 0, 1, 0, nullptr},
  {"Colour", "", 0, 0, 1, 1, COLOURS},
  {"Drive", "", 1, 1, OVERLOAD, 0, nullptr},
  {"Cutoff", "Hz", 1000, LOWEST, HIGHEST, 0, nullptr},
  {"Emphasis", "", 0.3f, 0, 1, 0, nullptr},
  {"Amount", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Tracking", "", 1, 0, TRACKS, TRACKS, FRACTIONS},
  {"Sweep", "s", 0.01f, QUICKEST, SLOWEST, 0, nullptr},
  {"Close", "s", 0.4f, QUICKEST, SLOWEST, 0, nullptr},
  {"Floor", "", 0.2f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.8f, 0, 1, 0, nullptr},
  {"Release", "", 1, 0, 1, 1, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::LADDER
