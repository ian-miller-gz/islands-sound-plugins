// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::WAVETABLE {

constexpr Whole TABLES = 16;
constexpr Whole SHAPES = 5;
constexpr Whole STACKS = 3;
constexpr Whole CHOICES = 3;

inline constexpr STRING::Hot NAMES[] = {
  "Sine",  "Harmonic", "Saw",     "Square",  "Hollow", "Triangle",
  "Pulse", "Organ",    "Octaves", "Formant", "Vowel",  "Comb",
  "Sync",  "Prime",    "Scatter", "Resonant"};
static_assert(std::size(NAMES) == TABLES);

inline constexpr STRING::Hot WAVES[] = {"Sine",   "Triangle", "Saw",
                                        "Square", "Sample",   "Random"};
static_assert(std::size(WAVES) == SHAPES + 1);

inline constexpr STRING::Hot UNISONS[] = {"1", "2", "3", "4"};
static_assert(std::size(UNISONS) == STACKS + 1);

inline constexpr STRING::Hot WARPS[] = {"Bend", "Sync", "Knee", "Pulse"};
static_assert(std::size(WARPS) == CHOICES + 1);

inline constexpr STRING::Hot MODES[] = {"24dB", "12dB", "Band", "High"};
static_assert(std::size(MODES) == CHOICES + 1);

}  // namespace SOUND::PLUGINS::WAVETABLE
