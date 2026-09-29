// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "mono.indices.hpp"

namespace SOUND::MONO {

constexpr Whole FEET = 3;
constexpr Whole THREEWAY = 2;
constexpr Whole FOURWAY = 3;
constexpr Float CENTS = 100;
constexpr Float SEMITONES = 12;
constexpr Float OCTAVES = 5;
constexpr Float SWING = 3;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10;
constexpr Float LONGEST = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float SLOW = 0.1f;
constexpr Float FAST = 30;

inline constexpr STRING::Hot SLIDES[] = {"Off", "Auto", "On"};
inline constexpr STRING::Hot WAVES[] = {
  "Triangle", "Square", "Random", "Noise"};
inline constexpr STRING::Hot RANGES[] = {"16", "8", "4", "2"};
inline constexpr STRING::Hot SOURCES[] = {"LFO", "Manual", "Envelope"};
inline constexpr STRING::Hot DOWNS[] = {"One", "Two", "Pulse"};
inline constexpr STRING::Hot TRIGGERS[] = {"LFO", "Gate", "Trigger"};
inline constexpr STRING::Hot AMPLIFIERS[] = {"Envelope", "Gate"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Portamento", "s", 0.1f, 0, LONGEST, 0, nullptr},
  {"Slide", "", 0, 0, THREEWAY, THREEWAY, SLIDES},
  {"Rate", "Hz", 5, SLOW, FAST, 0, nullptr},
  {"Wave", "", 0, 0, FOURWAY, FOURWAY, WAVES},
  {"Vibrato", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Range", "", 1, 0, FEET, FEET, RANGES},
  {"Width", "", 0, 0, 1, 0, nullptr},
  {"Source", "", 1, 0, THREEWAY, THREEWAY, SOURCES},
  {"Pulse", "", 0, 0, 1, 0, nullptr},
  {"Saw", "", 0.8f, 0, 1, 0, nullptr},
  {"Sub", "", 0.4f, 0, 1, 0, nullptr},
  {"Down", "", 0, 0, THREEWAY, THREEWAY, DOWNS},
  {"Noise", "", 0, 0, 1, 0, nullptr},
  {"Cutoff", "Hz", 1200, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.3f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Modulation", "oct", 0, 0, SWING, 0, nullptr},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.6f, 0, 1, 0, nullptr},
  {"Release", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Trigger", "", 2, 0, THREEWAY, THREEWAY, TRIGGERS},
  {"Amplifier", "", 0, 0, 1, 1, AMPLIFIERS},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::MONO
