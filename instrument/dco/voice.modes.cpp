// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SHELF = 100;
constexpr Float BOOST = 6;
constexpr Float LOWER = 240;
constexpr Float UPPER = 720;
constexpr Whole FLATTENED = 1;

constexpr Float MILLISECOND = 0.001f;
constexpr Float WIDE = 1.85f;
constexpr Float NARROW = 0.2f;
constexpr Float SLOW = 0.513f;
constexpr Float FAST = 0.863f;
constexpr Float QUICK = 9.75f;
constexpr Float WET = 0.5f;
constexpr Whole OFF = 0;

struct Position {
  Whole kind;
  Float cutoff;
  Float gain;
};

constexpr Position POSITIONS[] = {
  {CORE::FILTER::LOWSHELF, SHELF, BOOST},
  {CORE::FILTER::LOWSHELF, SHELF, 0},
  {CORE::FILTER::HIGH, LOWER, 0},
  {CORE::FILTER::HIGH, UPPER, 0}};
static_assert(std::size(POSITIONS) == std::size(DCO::POSITIONS));

struct Mode {
  Float rate;
  Float depth;
  Float wet;
};

constexpr Mode MODES[] = {
  {SLOW, WIDE, 0}, {SLOW, WIDE, WET}, {FAST, WIDE, WET}, {QUICK, NARROW, WET}};
static_assert(std::size(MODES) == std::size(DCO::CHORUSES));

}  // namespace

void SOUND::PLUGINS::DCO::settle(
  CORE::FILTER::Biquad &highpass, Whole position, Whole rate) {
  const Position &chosen =
    ::POSITIONS[position < std::size(::POSITIONS) ? position : ::FLATTENED];
  CORE::FILTER::settle(
    highpass, chosen.kind, chosen.cutoff, CORE::FILTER::FLAT, chosen.gain,
    rate);
}

void SOUND::PLUGINS::DCO::settle(
  CORE::LINE::Chorus &chorus, Whole mode, Whole rate) {
  const Mode &chosen = ::MODES[mode < std::size(::MODES) ? mode : ::OFF];
  CORE::LINE::settle(
    chorus, chosen.rate, chosen.depth * ::MILLISECOND, chosen.wet, rate);
}
