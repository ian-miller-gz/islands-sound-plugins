// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float QUIET = 0;
constexpr Float INSTANT = 0;
constexpr Float SNAP = 0.005f;
constexpr Float CRISP = 2000.0f;
constexpr Float ROUND = 1000.0f;
constexpr Float PUSH = 0.5f;
constexpr Float POSITIVE = 0.001f;
constexpr Float GAP = 0.0005f;
constexpr Float NEGATIVE = 0.001f;
constexpr Float PLATEAU = 0.003f;
constexpr Float PEAK = 1.0f;
constexpr Float TROUGH = -0.5f;
constexpr Float SHELF = 0.15f;
constexpr Float SPAN = POSITIVE + GAP + NEGATIVE + PLATEAU;

struct Segment {
  Float until;
  Float level;
};

constexpr Segment SEGMENTS[] = {
  {POSITIVE, PEAK},
  {POSITIVE + GAP, QUIET},
  {POSITIVE + GAP + NEGATIVE, TROUGH},
  {SPAN, SHELF}};

auto shaped(Float elapsed) -> Float {
  for (const Segment &segment : SEGMENTS)
    if (elapsed < segment.until) return segment.level;
  return QUIET;
}

auto noise(PERCUSSION::Kick &kick) -> Float {
  const Float snap = ENVELOPE::tick(kick.click, kick.snap);
  const Float noise = FILTER::tick(kick.crisp, NOISE::tick(kick.white));
  return noise * snap * kick.attack * kick.velocity;
}

auto pulse(PERCUSSION::Kick &kick) -> Float {
  const Float left = ENVELOPE::tick(kick.bend, kick.pulse);
  const Float level = ::shaped((ONE - left) * SPAN);
  const Float round = FILTER::tick(kick.round, level);
  const Float pushed = round * kick.attack * kick.velocity;
  PERCUSSION::strike(kick.body.body, pushed * kick.push);
  return pushed;
}

struct Kind {
  ENVELOPE::Gate PERCUSSION::Kick::*gate;
  ENVELOPE::Envelope PERCUSSION::Kick::*envelope;
  auto (*sound)(PERCUSSION::Kick &kick) -> Float;
};

constexpr Kind KINDS[PERCUSSION::TRANSIENT::TRANSIENTS] = {
  {&PERCUSSION::Kick::click, &PERCUSSION::Kick::snap, noise},
  {&PERCUSSION::Kick::bend, &PERCUSSION::Kick::pulse, pulse}};

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::shape(
  Kick &kick, Whole kind, Whole rate) {
  const Whole held = kind < TRANSIENT::TRANSIENTS ? kind : TRANSIENT::NOISE;
  if (held != kick.kind) {
    ENVELOPE::choke(kick.click);
    ENVELOPE::choke(kick.bend);
  }
  kick.kind = held;
  shape(kick.snap, SNAP, rate);
  kick.pulse = ENVELOPE::AD::create(INSTANT, SPAN);
  ENVELOPE::shape(kick.pulse, rate);
  FILTER::settle(kick.crisp, FILTER::HIGH, CRISP, rate);
  FILTER::settle(kick.round, FILTER::LOW, ROUND, rate);
  kick.push = PUSH / ENVELOPE::frames(POSITIVE, rate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::crack(Kick &kick) -> Float {
  return ::KINDS[kick.kind].sound(kick);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Kick &kick, Float hertz, Float velocity) {
  const Float held = hertz > kick.top ? kick.top : hertz;
  kick.base = turn(held, kick.rate);
  kick.velocity = struck(velocity, kick.curve);
  darken(kick);
  const ::Kind &kind = ::KINDS[kick.kind];
  ENVELOPE::strike(kick.*kind.gate, kick.*kind.envelope, kick.velocity);
  kick.breath = 0;
  strike(kick.body, kick.base, kick.velocity);
}
