// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "bull.indices.hpp"

namespace SOUND::PLUGINS::BULL {

constexpr Whole OSCILLATORS = 2;
constexpr Whole FOURWAY = 3;
constexpr Whole THREEWAY = 2;
constexpr Whole TWOWAY = 1;
constexpr Float CENTS = 100;
constexpr Float BEATING = 50;
constexpr Float OCTAVES = 5;
constexpr Float OVERLOAD = 4;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10;
constexpr Float LONGEST = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;

inline constexpr STRING::Hot VOICES[] = {"Bass", "Horn", "Deep", "Manual"};
inline constexpr STRING::Hot RANGES[] = {"Low", "Full"};
inline constexpr STRING::Hot FEET[] = {"32", "16", "8"};
inline constexpr STRING::Hot WAVES[] = {"Saw", "Square"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Voice", "", 0, 0, FOURWAY, FOURWAY, VOICES},
  {"Range", "", 0, 0, TWOWAY, TWOWAY, RANGES},
  {"Octave", "", 1, 0, THREEWAY, THREEWAY, FEET},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"WaveA", "", 0, 0, TWOWAY, TWOWAY, WAVES},
  {"WaveB", "", 0, 0, TWOWAY, TWOWAY, WAVES},
  {"Beat", "ct", 0, -BEATING, BEATING, 0, nullptr},
  {"Sub", "", 0.3f, 0, 1, 0, nullptr},
  {"Cutoff", "Hz", 300, LOWEST, HIGHEST, 0, nullptr},
  {"Emphasis", "", 0.2f, 0, 1, 0, nullptr},
  {"Contour", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Sweep", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Close", "s", 0.5f, QUICKEST, SLOWEST, 0, nullptr},
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 1, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 1, 0, 1, 1, nullptr},
  {"Drive", "", 1, 1, OVERLOAD, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::BULL
