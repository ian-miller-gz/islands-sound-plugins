// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole BIT = 1;

using Pick = auto (*)(Whole at) -> const RHYTHM::Pattern &;

struct Place {
  Pick pick;
  Whole at;
};

constexpr Pick STRAIGHT = RHYTHM::STRAIGHT::pattern;
constexpr Pick TRIPLE = RHYTHM::TRIPLE::pattern;

constexpr Place PLACES[RHYTHM::PATTERNS] = {
  {STRAIGHT, 0}, {STRAIGHT, 1},  {STRAIGHT, 2},  {STRAIGHT, 3},
  {STRAIGHT, 4}, {STRAIGHT, 5},  {STRAIGHT, 6},  {STRAIGHT, 7},
  {TRIPLE, 0},   {TRIPLE, 1},    {TRIPLE, 2},    {STRAIGHT, 8},
  {STRAIGHT, 9}, {STRAIGHT, 10}, {STRAIGHT, 11}, {STRAIGHT, 12}};

auto lit(Whole lane, Whole steps, Whole step) -> Flag {
  if (step >= steps) return false;
  return ((lane >> (steps - ::BIT - step)) & ::BIT) != 0;
}

}  // namespace

auto SOUND::PLUGINS::RHYTHM::pattern(Whole index) -> const Pattern & {
  const ::Place &place = ::PLACES[index < PATTERNS ? index : 0];
  return place.pick(place.at);
}

auto SOUND::PLUGINS::RHYTHM::length(const Pattern &pattern) -> Whole {
  return pattern.beats * pattern.division;
}

auto SOUND::PLUGINS::RHYTHM::struck(
  const Pattern &pattern, Whole voice, Whole step) -> Flag {
  if (voice >= VOICES) return false;
  return ::lit(pattern.lanes[voice], length(pattern), step);
}

auto SOUND::PLUGINS::RHYTHM::accented(const Pattern &pattern, Whole step)
  -> Flag {
  return ::lit(pattern.accent, length(pattern), step);
}
