// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::HARNESS::PERCUSSION {
void sweep();
void slopes();
void kick();
void play();
}  // namespace SOUND::PLUGINS::HARNESS::PERCUSSION

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole RATE = 48000;
constexpr Float HALF = 0.5f;
constexpr Float OVER = 2.0f;
constexpr Float LIGHT = 1.0f;
constexpr Float HEAVY = -1.0f;
constexpr Float LINEAR = 0;
constexpr Float HERTZ = 1000.0f;
constexpr Float FULL = 1.0f;
constexpr Float OCTAVES = 2.0f;
constexpr Float FADE = 0.01f;
constexpr Whole OPEN = 100;
constexpr Whole LONGEST = RATE;

void struck() {
  std::printf(
    "struck %.9g %.9g %.9g %.9g\n",
    double(CORE::PERCUSSION::struck(HALF, LINEAR)),
    double(CORE::PERCUSSION::struck(HALF, LIGHT)),
    double(CORE::PERCUSSION::struck(HALF, HEAVY)),
    double(CORE::PERCUSSION::struck(OVER, LINEAR)));
}

void touched() {
  std::printf(
    "touched %.9g %.9g\n",
    double(CORE::PERCUSSION::touched(HERTZ, FULL, OCTAVES)),
    double(CORE::PERCUSSION::touched(HERTZ, HALF, OCTAVES)));
}

void choke() {
  CORE::PERCUSSION::Choke choke;
  CORE::PERCUSSION::settle(choke, FADE, RATE);
  CORE::PERCUSSION::open(choke);
  Whole whole = 0;
  for (Whole at = 0; at < OPEN; ++at)
    whole += CORE::PERCUSSION::tick(choke) == FULL;
  CORE::PERCUSSION::close(choke);
  Whole closed = 0;
  while (closed < LONGEST && CORE::PERCUSSION::tick(choke) > 0) ++closed;
  std::printf("open %zu\n", whole);
  std::printf("closed %zu\n", closed + 1);
  std::printf("after %d\n", CORE::PERCUSSION::sounding(choke));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::PERCUSSION {

void run() {
  ::struck();
  ::touched();
  ::choke();
  sweep();
  slopes();
  kick();
  play();
}

}  // namespace SOUND::PLUGINS::HARNESS::PERCUSSION
