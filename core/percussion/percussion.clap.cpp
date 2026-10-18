// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float WIDTH = 1.2f;
constexpr Float FLAT = 0;
constexpr Float TAIL = 0.6f;
constexpr Float GAIN = 2.0f;
constexpr Float SPAN = 0.5f;
constexpr Float ONE = 1.0f;
constexpr Whole SHORTEST = 1;
constexpr Whole SINGLE = 1;

void space(PERCUSSION::Clap &clap) {
  const Float scatter = clap.hand.scatter;
  const Float drawn = scatter > 0 ? NOISE::tick(clap.white) : 0;
  const Float frames = clap.span * (ONE + scatter * SPAN * drawn);
  const Whole gap = Whole(frames);
  clap.next = gap < SHORTEST ? SHORTEST : gap;
}

void advance(PERCUSSION::Clap &clap) {
  clap.burst *= clap.fall;
  if (clap.left == 0 || ++clap.clock < clap.next) return;
  clap.clock = 0;
  clap.burst = clap.velocity;
  ::space(clap);
  if (--clap.left == 0) ENVELOPE::strike(clap.gate, clap.tail, clap.velocity);
}

auto low(PERCUSSION::Clap &clap, Float in) -> Float {
  return clap.open ? in : FILTER::tick(clap.low, in);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clap &clap, Float spread, Float decay, Float tone, Whole rate) {
  settle(
    clap, Hand{.spread = spread, .decay = decay, .tone = tone, .tail = tone},
    rate);
  clap.open = true;
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Clap &clap, const Hand &hand, Whole rate) {
  const Whole gap = Whole(hand.spread * Float(rate));
  clap.hand = hand;
  clap.hand.bursts = hand.bursts < SINGLE ? SINGLE : hand.bursts;
  clap.gap = gap < SHORTEST ? SHORTEST : gap;
  clap.fall = fall(hand.spread, rate);
  shape(clap.tail, hand.decay, rate);
  FILTER::settle(clap.band, FILTER::BAND, hand.tone, WIDTH, FLAT, rate);
  FILTER::settle(clap.ring, FILTER::BAND, hand.tail, WIDTH, FLAT, rate);
  FILTER::settle(clap.low, FILTER::LOW, hand.cutoff, hand.emphasis, rate);
  clap.open = false;
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Clap &clap, Float velocity) {
  clap.velocity = struck(velocity, clap.hand.curve);
  clap.burst = clap.velocity;
  clap.clock = 0;
  clap.span = touched(Float(clap.gap), clap.velocity, -clap.hand.touch);
  ::space(clap);
  clap.left = clap.hand.bursts - SINGLE;
  ENVELOPE::choke(clap.gate);
  if (clap.left == 0) ENVELOPE::strike(clap.gate, clap.tail, clap.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Clap &clap) -> Float {
  if (!sounding(clap)) return 0;
  const Float tail = ENVELOPE::tick(clap.gate, clap.tail) * ::TAIL;
  const Float burst = clap.burst;
  ::advance(clap);
  const Float noise = NOISE::tick(clap.white);
  const Float bursts = FILTER::tick(clap.band, noise) * burst;
  const Float ring = FILTER::tick(clap.ring, noise) * tail * clap.velocity;
  return SHAPER::soft(::low(clap, bursts + ring) * GAIN);
}
