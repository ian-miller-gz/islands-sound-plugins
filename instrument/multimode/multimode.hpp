// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "multimode.indices.hpp"

namespace SOUND::PLUGINS::MULTIMODE {

constexpr Float CENTS = 200;
constexpr Float DETUNING = 50;
constexpr Float SEMITONES = 12;
constexpr Float INTERVAL = 24;
constexpr Float OCTAVES = 5;
constexpr Float DEPTH = 3;
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 20;
constexpr Float NARROW = 0.05f;
constexpr Float SQUARE = 0.5f;
constexpr Float QUICKEST = 0.001f;
constexpr Float LONGEST = 10;
constexpr Float GLIDING = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;

inline constexpr STRING::Hot SOURCES[] = {"LFO", "Envelope"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Glide", "s", 0, 0, GLIDING, 0, nullptr},
  {"Frequency1", "st", 0, -INTERVAL, INTERVAL, 0, nullptr},
  {"Shape1", "", 0, 0, 1, 0, nullptr},
  {"Level1", "", 0.6f, 0, 1, 0, nullptr},
  {"Frequency2", "st", 0.07f, -INTERVAL, INTERVAL, 0, nullptr},
  {"Shape2", "", 0, 0, 1, 0, nullptr},
  {"Level2", "", 0.6f, 0, 1, 0, nullptr},
  {"Sync", "", 0, 0, 1, 1, nullptr},
  {"Width", "", SQUARE, NARROW, SQUARE, 0, nullptr},
  {"Sweep", "", 0, 0, 1, 0, nullptr},
  {"Source", "", 0, 0, 1, 1, SOURCES},
  {"Modulation", "st", 0, 0, SEMITONES, 0, nullptr},
  {"Modulator", "", 0, 0, 1, 1, SOURCES},
  {"Rate", "Hz", 4, SLOWEST, FASTEST, 0, nullptr},
  {"Cutoff", "Hz", 1500, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Mode", "", 0, 0, 1, 0, nullptr},
  {"Band", "", 0, 0, 1, 1, nullptr},
  {"Envelope", "oct", 2, -OCTAVES, OCTAVES, 0, nullptr},
  {"Wobble", "oct", 0, 0, DEPTH, 0, nullptr},
  {"Tracking", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack1", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay1", "s", 0.4f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain1", "", 0.3f, 0, 1, 0, nullptr},
  {"Attack2", "s", 0.005f, QUICKEST, LONGEST, 0, nullptr},
  {"Decay2", "s", 0.3f, QUICKEST, LONGEST, 0, nullptr},
  {"Sustain2", "", 0.8f, 0, 1, 0, nullptr},
  {"Unison", "", 0, 0, 1, 1, nullptr},
  {"Detune", "ct", 10, 0, DETUNING, 0, nullptr},
  {"Spread", "", 0.5f, 0, 1, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::MULTIMODE
