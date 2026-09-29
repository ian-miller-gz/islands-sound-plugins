// SPDX-License-Identifier: AGPL-3.0-or-later
#include "clock.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float UNIT = 1.0f;
constexpr Whole PAIR = 2;

}  // namespace

auto SOUND::CORE::CLOCK::frames(Float tempo, Float division, Whole rate)
  -> Float {
  const Float steps = tempo * division;
  const Float held = steps < SLOWEST ? SLOWEST : steps;
  const Float span = Float(rate) * MINUTE / held;
  return span < UNIT ? UNIT : span;
}

void SOUND::CORE::CLOCK::settle(
  Clock &clock, Float tempo, Float division, Float swing, Whole rate) {
  const Float span = frames(tempo, division, rate);
  if (clock.frames > 0) clock.left *= span / clock.frames;
  clock.rate = rate;
  clock.frames = span;
  clock.swing = swing < 0 ? 0 : swing > LOOSEST ? LOOSEST : swing;
}

auto SOUND::CORE::CLOCK::length(const Clock &clock, Whole step) -> Float {
  const Float lean = step % ::PAIR == 0 ? clock.swing : -clock.swing;
  return clock.frames * (UNIT + lean);
}

auto SOUND::CORE::CLOCK::position(const Clock &clock) -> Float {
  if (clock.step == 0) return 0;
  const Float span = length(clock, clock.step - 1);
  if (span <= 0) return 0;
  const Float done = UNIT - clock.left / span;
  return done < 0 ? 0 : done;
}

void SOUND::CORE::CLOCK::run(Clock &clock) { clock.running = true; }

void SOUND::CORE::CLOCK::stop(Clock &clock) { clock.running = false; }

void SOUND::CORE::CLOCK::reset(Clock &clock) {
  clock.left = 0;
  clock.step = 0;
}

auto SOUND::CORE::CLOCK::advance(
  Clock &clock, Whole frames, Edge *edges, Whole room) -> Whole {
  if (!clock.running || clock.frames <= 0) return 0;
  Whole count = 0;
  Float at = clock.left;
  while (at < Float(frames)) {
    if (count < room) edges[count++] = {Whole(at), clock.step};
    at += length(clock, clock.step);
    ++clock.step;
  }
  clock.left = at - Float(frames);
  return count;
}
