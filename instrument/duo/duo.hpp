// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "duo.indices.hpp"

namespace SOUND::DUO {

constexpr Float CENTS = 200;
constexpr Float DETUNE = 100;
constexpr Float SEMITONES = 12;
constexpr Float INTERVAL = 24;
constexpr Whole INTERVALS = 48;
constexpr Float OCTAVES = 5;
constexpr Float DEPTH = 3;
constexpr Float SLOWEST = 0.2f;
constexpr Float FASTEST = 20;
constexpr Float NARROW = 0.05f;
constexpr Float SQUARE = 0.5f;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 10;
constexpr Float GLIDING = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float BOTTOM = 10;
constexpr Float TOP = 10000;

inline constexpr STRING::Hot WAVES[] = {"Saw", "Square"};
inline constexpr STRING::Hot PULSES[] = {"Saw", "Pulse"};
inline constexpr STRING::Hot SOURCES[] = {"LFO", "ADSR"};
inline constexpr STRING::Hot COLOURS[] = {"White", "Pink"};
inline constexpr STRING::Hot CLOCKS[] = {"LFO", "Keyboard"};
inline constexpr STRING::Hot SLOPES[] = {"12dB", "24dB"};
inline constexpr STRING::Hot CONTOURS[] = {"ADSR", "AR"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Glide", "s", 0, 0, GLIDING, 0, nullptr},
  {"Coarse1", "st", 0, -INTERVAL, INTERVAL, INTERVALS, nullptr},
  {"Keyboard", "", 1, 0, 1, 1, nullptr},
  {"Wave1", "", 0, 0, 1, 1, WAVES},
  {"Level1", "", 0.6f, 0, 1, 0, nullptr},
  {"Coarse2", "st", 0, -INTERVAL, INTERVAL, INTERVALS, nullptr},
  {"Fine2", "ct", 7, -DETUNE, DETUNE, 0, nullptr},
  {"Wave2", "", 0, 0, 1, 1, PULSES},
  {"Level2", "", 0.6f, 0, 1, 0, nullptr},
  {"Sync", "", 0, 0, 1, 1, nullptr},
  {"Width", "", SQUARE, NARROW, SQUARE, 0, nullptr},
  {"Sweep", "", 0, 0, 1, 0, nullptr},
  {"Source", "", 0, 0, 1, 1, SOURCES},
  {"Ring", "", 0, 0, 1, 0, nullptr},
  {"Noise", "", 0, 0, 1, 0, nullptr},
  {"Colour", "", 0, 0, 1, 1, COLOURS},
  {"Rate", "Hz", 5, SLOWEST, FASTEST, 0, nullptr},
  {"Vibrato", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr},
  {"Clock", "", 0, 0, 1, 1, CLOCKS},
  {"Lag", "s", 0, 0, 1, 0, nullptr},
  {"Sample", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Slope", "", 1, 0, 1, 1, SLOPES},
  {"Cutoff", "Hz", 2000, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Tracking", "", 0.5f, 0, 1, 0, nullptr},
  {"Wobble", "oct", 0, 0, DEPTH, 0, nullptr},
  {"Scatter", "oct", 0, 0, DEPTH, 0, nullptr},
  {"Amount", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Contour", "", 0, 0, 1, 1, CONTOURS},
  {"Highpass", "Hz", BOTTOM, BOTTOM, TOP, 0, nullptr},
  {"Attack", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay", "s", 0.3f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain", "", 0.7f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, LONGEST, 0, nullptr},
  {"Attack2", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Release2", "s", 0.5f, QUICKEST, LONGEST, 0, nullptr},
  {"Amplifier", "", 0, 0, 1, 1, CONTOURS},
  {"Repeat", "", 0, 0, 1, 1, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::DUO
