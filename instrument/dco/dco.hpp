// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "dco.indices.hpp"

namespace SOUND::DCO {

constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;
constexpr Float OCTAVES = 5;
constexpr Float DEPTH = 3;
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 20;
constexpr Float WAITING = 3;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 10;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Whole THREEWAY = 2;
constexpr Whole FOURWAY = 3;

inline constexpr STRING::Hot FEET[] = {"16", "8", "4"};
inline constexpr STRING::Hot SOURCES[] = {"LFO", "Manual", "Envelope"};
inline constexpr STRING::Hot POSITIONS[] = {"0", "1", "2", "3"};
inline constexpr STRING::Hot POLARITIES[] = {"Positive", "Negative"};
inline constexpr STRING::Hot MODES[] = {"Envelope", "Gate"};
inline constexpr STRING::Hot CHORUSES[] = {"Off", "I", "II", "I+II"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Range", "", 1, 0, THREEWAY, THREEWAY, FEET},
  {"Vibrato", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Pulse", "", 0, 0, 1, 1, nullptr},
  {"Saw", "", 1, 0, 1, 1, nullptr},
  {"Width", "", 0, 0, 1, 0, nullptr},
  {"Source", "", 0, 0, THREEWAY, THREEWAY, SOURCES},
  {"Sub", "", 0.3f, 0, 1, 0, nullptr},
  {"Noise", "", 0, 0, 1, 0, nullptr},
  {"Highpass", "", 1, 0, FOURWAY, FOURWAY, POSITIONS},
  {"Cutoff", "Hz", 1500, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 2, 0, OCTAVES, 0, nullptr},
  {"Polarity", "", 0, 0, 1, 1, POLARITIES},
  {"Wobble", "oct", 0, 0, DEPTH, 0, nullptr},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Amplifier", "", 0, 0, 1, 1, MODES},
  {"Attack", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay", "s", 0.5f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain", "", 0.6f, 0, 1, 0, nullptr},
  {"Release", "s", 0.4f, QUICKEST, LONGEST, 0, nullptr},
  {"Rate", "Hz", 4, SLOWEST, FASTEST, 0, nullptr},
  {"Delay", "s", 0, 0, WAITING, 0, nullptr},
  {"Chorus", "", 1, 0, FOURWAY, FOURWAY, CHORUSES},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::DCO
