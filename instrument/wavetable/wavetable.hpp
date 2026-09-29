// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "wavetable.indices.hpp"

namespace SOUND::WAVETABLE {

constexpr Whole TABLES = 16;
constexpr Whole FRAMES = 64;
constexpr Float LAST = Float(FRAMES - 1);
constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;
constexpr Float INTERVALS = 24;
constexpr Whole SEMITONE = 2 * Whole(INTERVALS);
constexpr Float BEATING = 50;
constexpr Float OCTAVES = 5;
constexpr Float SLOWEST = 0.05f;
constexpr Float FASTEST = 20;
constexpr Float VIBRATING = 2;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 10;
constexpr Float GLIDING = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Whole LAYOUTS = TABLES - 1;
constexpr Whole SHAPES = 5;
constexpr Float SAW = 2;
constexpr Float FORMANT = 9;
constexpr Float OPENED = 8;
constexpr Float SWEPT = 24;

inline constexpr STRING::Hot NAMES[] = {
  "Sine",  "Harmonic", "Saw",     "Square",  "Hollow", "Triangle",
  "Pulse", "Organ",    "Octaves", "Formant", "Vowel",  "Comb",
  "Sync",  "Prime",    "Scatter", "Resonant"};
static_assert(std::size(NAMES) == TABLES);

inline constexpr STRING::Hot WAVES[] = {"Sine",   "Triangle", "Saw",
                                        "Square", "Sample",   "Random"};
static_assert(std::size(WAVES) == SHAPES + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Table1", "", SAW, 0, LAYOUTS, LAYOUTS, NAMES},
  {"Position1", "", 0, 0, LAST, 0, nullptr},
  {"Table2", "", FORMANT, 0, LAYOUTS, LAYOUTS, NAMES},
  {"Position2", "", OPENED, 0, LAST, 0, nullptr},
  {"Interval", "st", 0, -INTERVALS, INTERVALS, SEMITONE, nullptr},
  {"Detune", "ct", 7, -BEATING, BEATING, 0, nullptr},
  {"Mix", "", 0.5f, 0, 1, 0, nullptr},
  {"Sweep", "", SWEPT, -LAST, LAST, 0, nullptr},
  {"Scan", "", 0, 0, LAST, 0, nullptr},
  {"Rate", "Hz", 0.5f, SLOWEST, FASTEST, 0, nullptr},
  {"Wave", "", 1, 0, SHAPES, SHAPES, WAVES},
  {"Vibrato", "st", 0, 0, VIBRATING, 0, nullptr},
  {"Cutoff", "Hz", 5000, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 1, 0, OCTAVES, 0, nullptr},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack1", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay1", "s", 1.2f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain1", "", 0.3f, 0, 1, 0, nullptr},
  {"Release1", "s", 0.6f, QUICKEST, LONGEST, 0, nullptr},
  {"Attack2", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay2", "s", 0.8f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain2", "", 0.7f, 0, 1, 0, nullptr},
  {"Release2", "s", 0.5f, QUICKEST, LONGEST, 0, nullptr},
  {"Velocity", "", 0.5f, 0, 1, 0, nullptr},
  {"Glide", "s", 0, 0, GLIDING, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::WAVETABLE
