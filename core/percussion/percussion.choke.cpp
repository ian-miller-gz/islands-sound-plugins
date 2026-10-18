// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {

constexpr Float SHORTEST = 0.001f;
constexpr Float LONGEST = 1.0f;
constexpr Float FULL = 1.0f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Choke &choke, Float seconds, Whole rate) {
  const Float held = seconds < SHORTEST  ? SHORTEST
                     : seconds > LONGEST ? LONGEST
                                         : seconds;
  const Float frames = held * Float(rate);
  choke.step = frames > FULL ? FULL / frames : FULL;
}

void SOUND::PLUGINS::CORE::PERCUSSION::open(Choke &choke) {
  choke.level = FULL;
  choke.closed = false;
}

void SOUND::PLUGINS::CORE::PERCUSSION::close(Choke &choke) {
  choke.closed = true;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Choke &choke) -> Float {
  if (!choke.closed) return choke.level;
  const Float level = choke.level - choke.step;
  choke.level = level > 0 ? level : 0;
  return choke.level;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Choke &choke) -> Flag {
  return choke.level > 0;
}
