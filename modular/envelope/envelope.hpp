// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::ENVELOPE {

constexpr Whole SHAPE = 0;
constexpr Whole ATTACK = 1;
constexpr Whole DECAY = 2;
constexpr Whole SUSTAIN = 3;
constexpr Whole RELEASE = 4;
constexpr Whole CURVE = 5;
constexpr Whole TRIGGER = 6;
constexpr Whole VELOCITY = 7;
constexpr Whole DEPTH = 8;
constexpr Whole DELAY = 9;
constexpr Whole HOLD = 10;
constexpr Whole RISE = 11;
constexpr Whole FALL = 12;
constexpr Whole LOOP = 13;
constexpr Whole TIME = 14;
constexpr Whole PARAMETERS = 15;

constexpr Whole ADSR = 0;
constexpr Whole GATE = 3;
constexpr Whole EXPONENTIAL = 1;
constexpr Whole RESUME = 1;
constexpr Whole OFF = 0;
constexpr Whole FREE = 2;

constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10.0f;
constexpr Float OCTAVES = 4.0f;
constexpr Float EVEN = 0.5f;

inline constexpr STRING::Hot SHAPES[] = {"ADSR", "AD", "AR", "Gate"};
inline constexpr STRING::Hot CURVES[] = {"Linear", "Exponential"};
inline constexpr STRING::Hot TRIGGERS[] = {"Reset", "Resume"};
inline constexpr STRING::Hot LOOPS[] = {"Off", "Held", "Free"};
static_assert(std::size(SHAPES) == GATE + 1);
static_assert(std::size(CURVES) == EXPONENTIAL + 1);
static_assert(std::size(TRIGGERS) == RESUME + 1);
static_assert(std::size(LOOPS) == FREE + 1);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Shape", "", ADSR, ADSR, GATE, GATE, SHAPES},
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.7f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Curve", "", EXPONENTIAL, 0, EXPONENTIAL, EXPONENTIAL, CURVES},
  {"Trigger", "", RESUME, 0, RESUME, RESUME, TRIGGERS},
  {"Velocity", "", 0.5f, 0, 1, 0, nullptr},
  {"Depth", "", 1, 0, 1, 0, nullptr},
  {"Delay", "s", 0, 0, SLOWEST, 0, nullptr},
  {"Hold", "s", 0, 0, SLOWEST, 0, nullptr},
  {"Rise", "", EVEN, 0, 1, 0, nullptr},
  {"Fall", "", 1, 0, 1, 0, nullptr},
  {"Loop", "", OFF, OFF, FREE, FREE, LOOPS},
  {"Time", "oct", 1, -OCTAVES, OCTAVES, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::ENVELOPE
