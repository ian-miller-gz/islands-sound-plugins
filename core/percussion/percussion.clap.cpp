// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float WIDTH = 1.2f;
constexpr Float FLAT = 0;
constexpr Float TAIL = 0.6f;
constexpr Float GAIN = 2.0f;
constexpr Whole SHORTEST = 1;

void advance(PERCUSSION::Clap &clap) {
  clap.burst *= clap.fall;
  if (clap.left == 0 || ++clap.clock < clap.gap) return;
  clap.clock = 0;
  clap.burst = clap.velocity;
  if (--clap.left == 0) ENVELOPE::strike(clap.gate, clap.tail, clap.velocity);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clap &clap, Float spread, Float decay, Float tone, Whole rate) {
  const Whole gap = Whole(spread * Float(rate));
  clap.gap = gap < SHORTEST ? SHORTEST : gap;
  clap.fall = fall(spread, rate);
  shape(clap.tail, decay, rate);
  FILTER::settle(clap.band, FILTER::BAND, tone, WIDTH, FLAT, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Clap &clap, Float velocity) {
  clap.velocity = struck(velocity);
  clap.burst = clap.velocity;
  clap.clock = 0;
  clap.left = BURSTS - 1;
  ENVELOPE::choke(clap.gate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Clap &clap) -> Float {
  const Float tail = ENVELOPE::tick(clap.gate, clap.tail) * ::TAIL;
  const Float level = clap.burst + tail * clap.velocity;
  ::advance(clap);
  const Float noise = FILTER::tick(clap.band, NOISE::tick(clap.white));
  return SHAPER::soft(noise * level * GAIN);
}
