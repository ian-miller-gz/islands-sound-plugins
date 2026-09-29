// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "supersaw.indices.hpp"

namespace SOUND::SUPERSAW {

constexpr Float CENTS = 200;
constexpr Float SEMITONES = 12;
constexpr Float OCTAVES = 5;
constexpr Float BEATING = 50;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10;
constexpr Float LONGEST = 3;
constexpr Float LOWEST = 20;
constexpr Float HIGHEST = 20000;
constexpr Float SINGLE = 1;
constexpr Float STACK = 4;
constexpr Whole STACKS = 3;

inline constexpr STRING::Hot UNISONS[] = {"1", "2", "3", "4"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "ct", 0, -CENTS, CENTS, 0, nullptr},
  {"Bend", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Detune", "", 0.5f, 0, 1, 0, nullptr},
  {"Mix", "", 0.6f, 0, 1, 0, nullptr},
  {"Cutoff", "Hz", 6000, LOWEST, HIGHEST, 0, nullptr},
  {"Resonance", "", 0.1f, 0, 1, 0, nullptr},
  {"Envelope", "oct", 1, 0, OCTAVES, 0, nullptr},
  {"Keyboard", "", 0.5f, 0, 1, 0, nullptr},
  {"Attack1", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay1", "s", 0.6f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain1", "", 0.4f, 0, 1, 0, nullptr},
  {"Release1", "s", 0.4f, QUICKEST, SLOWEST, 0, nullptr},
  {"Attack2", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay2", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain2", "", 0.8f, 0, 1, 0, nullptr},
  {"Release2", "s", 0.4f, QUICKEST, SLOWEST, 0, nullptr},
  {"Unison", "", SINGLE, SINGLE, STACK, STACKS, UNISONS},
  {"Spread", "ct", 10, 0, BEATING, 0, nullptr},
  {"Width", "", 0.5f, 0, 1, 0, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"Volume", "", 0.5f, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::SUPERSAW
