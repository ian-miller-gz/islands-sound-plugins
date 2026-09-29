// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "polymod.indices.hpp"

namespace SOUND::POLYMOD {

constexpr Float CENTS = 200;
constexpr Float QUARTER = 50;
constexpr Float DETUNING = 50;
constexpr Float SEMITONES = 12;
constexpr Float INTERVAL = 24;
constexpr Whole STEPS = 48;
constexpr Float OCTAVES = 5;
constexpr Float DEPTH = 3;
constexpr Float SLOWEST = 0.05f;
constexpr Float FASTEST = 20;
constexpr Float NARROW = 0.05f;
constexpr Float SQUARE = 0.5f;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 10;
constexpr Float GLIDING = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Whole THREEWAY = 2;

inline constexpr STRING::Hot TRACKS[] = {"Off", "Half", "Full"};
inline constexpr STRING::Hot WAVES[] = {"Triangle", "Saw", "Square"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Glide", "s", 0, 0, GLIDING, 0, nullptr},
  {"FrequencyA", "st", 0, -INTERVAL, INTERVAL, STEPS, nullptr},
  {"SawA", "", 1, 0, 1, 1, nullptr},
  {"PulseA", "", 0, 0, 1, 1, nullptr},
  {"WidthA", "", SQUARE, NARROW, SQUARE, 0, nullptr},
  {"Sync", "", 0, 0, 1, 1, nullptr},
  {"FrequencyB", "st", 0, -INTERVAL, INTERVAL, STEPS, nullptr},
  {"Fine", "ct", 7, -QUARTER, QUARTER, 0, nullptr},
  {"SawB", "", 1, 0, 1, 1, nullptr},
  {"TriangleB", "", 0, 0, 1, 1, nullptr},
  {"PulseB", "", 0, 0, 1, 1, nullptr},
  {"WidthB", "", SQUARE, NARROW, SQUARE, 0, nullptr},
  {"Low", "", 0, 0, 1, 1, nullptr},
  {"Keyboard", "", 1, 0, 1, 1, nullptr},
  {"LevelA", "", 0.7f, 0, 1, 0, nullptr},
  {"LevelB", "", 0.5f, 0, 1, 0, nullptr},
  {"Noise", "", 0, 0, 1, 0, nullptr},
  {"Cutoff", "Hz", 1200, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Amount", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Tracking", "", 1, 0, 1, THREEWAY, TRACKS},
  {"Attack1", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay1", "s", 0.5f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain1", "", 0.3f, 0, 1, 0, nullptr},
  {"Release1", "s", 0.4f, QUICKEST, LONGEST, 0, nullptr},
  {"Attack2", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay2", "s", 0.3f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain2", "", 0.8f, 0, 1, 0, nullptr},
  {"Release2", "s", 0.4f, QUICKEST, LONGEST, 0, nullptr},
  {"Contour", "", 0, 0, 1, 0, nullptr},
  {"Modulator", "", 0, 0, 1, 0, nullptr},
  {"Pitch", "", 0, 0, 1, 1, nullptr},
  {"Duty", "", 0, 0, 1, 1, nullptr},
  {"Filter", "", 0, 0, 1, 1, nullptr},
  {"Rate", "Hz", 5, SLOWEST, FASTEST, 0, nullptr},
  {"Wave", "", 0, 0, THREEWAY, THREEWAY, WAVES},
  {"Vibrato", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Wobble", "oct", 0, 0, DEPTH, 0, nullptr},
  {"Unison", "", 0, 0, 1, 1, nullptr},
  {"Detune", "ct", 10, 0, DETUNING, 0, nullptr},
  {"Release", "", 1, 0, 1, 1, nullptr},
  {"Hold", "", 0, 0, 1, 1, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::POLYMOD
