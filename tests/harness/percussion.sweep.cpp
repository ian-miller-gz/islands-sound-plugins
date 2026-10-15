// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cstdint>
#include <cstdio>

#include "../../core/percussion/percussion.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Whole RATE = 48000;
constexpr Whole SPAN = RATE / 2;
constexpr Float DECAY = 0.5f;
constexpr Float SECONDS = 0.1f;
constexpr Float DEPTH = 1.5f;
constexpr Float BASE = 100.0f;
constexpr Float FULL = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float FLAT = 0;
constexpr Float SLOPES[] = {1.0f, FLAT, -1.0f};
constexpr Whole LEVEL = 1;
constexpr Whole KICK = 4 * RATE;
constexpr Float TUNE = 55.0f;
constexpr Float ATTACK = 0.5f;
constexpr Float DRIVE = 0.2f;
constexpr Float TONE = 4000.0f;

auto bits(Float value) -> Bits { return std::bit_cast<Bits>(value); }

auto swept(CORE::PERCUSSION::Sweep &sweep) -> Float {
  const Float base = CORE::PERCUSSION::turn(BASE, RATE);
  CORE::PERCUSSION::strike(sweep, base, FULL);
  return base;
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::PERCUSSION {

void sweep() {
  CORE::PERCUSSION::Sweep five, six;
  CORE::PERCUSSION::settle(five, DECAY, SECONDS, DEPTH, RATE);
  CORE::PERCUSSION::settle(six, DECAY, SECONDS, DEPTH, FLAT, RATE);
  ::swept(five), ::swept(six);
  Whole differing = 0;
  for (Whole at = 0; at < SPAN; ++at) {
    differing += ::bits(CORE::PERCUSSION::tick(five)) !=
                 ::bits(CORE::PERCUSSION::tick(six));
    differing += ::bits(five.body.turn) != ::bits(six.body.turn);
  }
  std::printf("differing %zu\n", differing);
}

void slopes() {
  CORE::PERCUSSION::Sweep sweeps[std::size(SLOPES)];
  for (Whole at = 0; at < std::size(SLOPES); ++at) {
    CORE::PERCUSSION::settle(
      sweeps[at], DECAY, SECONDS, DEPTH, SLOPES[at], RATE);
    ::swept(sweeps[at]);
  }
  Float drop = FULL;
  for (Whole frame = 0; frame < SPAN && drop > HALF; ++frame) {
    for (CORE::PERCUSSION::Sweep &sweep : sweeps) CORE::PERCUSSION::tick(sweep);
    drop = sweeps[LEVEL].gate.level;
  }
  std::printf("drop %.9g\n", double(drop));
  std::printf("turns");
  for (const CORE::PERCUSSION::Sweep &sweep : sweeps)
    std::printf(" %.9g", double(sweep.body.turn));
  std::printf("\n");
}

void kick() {
  CORE::PERCUSSION::Kick kick;
  CORE::PERCUSSION::settle(kick, TUNE, DECAY, ATTACK, DRIVE, TONE, RATE);
  CORE::PERCUSSION::strike(kick, FULL);
  const Flag first = CORE::PERCUSSION::sounding(kick);
  Whole silent = KICK, loud = 0;
  for (Whole at = 0; at < KICK; ++at) {
    const Float out = CORE::PERCUSSION::tick(kick);
    if (silent == KICK && !CORE::PERCUSSION::sounding(kick)) silent = at;
    if (at > silent && out != 0) ++loud;
  }
  std::printf("kick %d %.9g %zu\n", first, double(silent) / RATE, loud);
}

}  // namespace SOUND::PLUGINS::HARNESS::PERCUSSION
