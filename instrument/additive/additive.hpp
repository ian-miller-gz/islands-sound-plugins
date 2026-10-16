// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "additive.indices.hpp"

namespace SOUND::PLUGINS::ADDITIVE {

constexpr Whole HARMONICS = 8;
constexpr Whole BANK = 32;
constexpr Whole SHAPES = 5;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 4.001f;
constexpr Float SLOPE = 12;
constexpr Float STRETCHING = 100;
constexpr Float NARROWEST = 0.5f;
constexpr Float WIDEST = 16;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float OCTAVES = 5;
constexpr Float REACH = 3;
constexpr Float SLOWEST = 0.05f;
constexpr Float FASTEST = 20;
constexpr Float VIBRATING = 2;
constexpr Float SCANNING = 16;
constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;
constexpr Float GLIDING = 3;

inline constexpr STRING::Hot WAVES[] = {"Sine",   "Triangle", "Saw",
                                        "Square", "Sample",   "Random"};
static_assert(std::size(WAVES) == SHAPES + 1);

constexpr auto harmonic(STRING::Hot name, Float order) -> CORE::TABLE::Row {
  return {name, "", 1 / order, 0, 1, 0, nullptr};
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.30f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.011f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay", "s", 0.1946f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain", "", 0.70f, 0, 1, 0, nullptr},
  {"Release", "s", 0.251f, QUICKEST, LONGEST, 0, nullptr},
  harmonic("Harmonic1", 1),
  harmonic("Harmonic2", 2),
  harmonic("Harmonic3", 3),
  harmonic("Harmonic4", 4),
  harmonic("Harmonic5", 5),
  harmonic("Harmonic6", 6),
  harmonic("Harmonic7", 7),
  harmonic("Harmonic8", 8),
  {"Partials", "", BANK, 1, BANK, BANK - 1, nullptr},
  {"Tilt", "dB", 0, -SLOPE, SLOPE, 0, nullptr},
  {"Balance", "", 0, -1, 1, 0, nullptr},
  {"Stretch", "ct", 0, -STRETCHING, STRETCHING, 0, nullptr},
  {"Centre", "", 6, 1, BANK, 0, nullptr},
  {"Breadth", "", 3, NARROWEST, WIDEST, 0, nullptr},
  {"Focus", "", 0.5f, 0, 1, 0, nullptr},
  {"Cutoff", "Hz", 2500, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.3f, 0, 1, 0, nullptr},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Sweep", "s", 0.011f, QUICKEST, LONGEST, 0, nullptr},
  {"Close", "s", 0.6f, QUICKEST, LONGEST, 0, nullptr},
  {"Velocity", "", 0.5f, 0, 1, 0, nullptr},
  {"Touch", "oct", 1, 0, REACH, 0, nullptr},
  {"Rate", "Hz", 0.5f, SLOWEST, FASTEST, 0, nullptr},
  {"Wave", "", 0, 0, SHAPES, SHAPES, WAVES},
  {"Vibrato", "st", 0, 0, VIBRATING, 0, nullptr},
  {"Scan", "", 2, 0, SCANNING, 0, nullptr},
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Glide", "s", 0, 0, GLIDING, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::ADDITIVE
