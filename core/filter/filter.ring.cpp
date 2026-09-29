// SPDX-License-Identifier: AGPL-3.0-or-later
#include "filter.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Whole SPARE = 2;

auto held(const FILTER::Ring &ring, Float delay) -> Float {
  const Float most = Float(ring.cells.size()) - Float(SPARE);
  return delay < ONE ? ONE : delay > most ? most : delay;
}

auto at(const FILTER::Ring &ring, Whole back) -> Float {
  const Whole size = ring.cells.size();
  return ring.cells[(ring.head + size - back) % size];
}

auto kept(Float value) -> Float {
  return value < -FILTER::STABLE  ? -FILTER::STABLE
         : value > FILTER::STABLE ? FILTER::STABLE
                                  : value;
}

}  // namespace

void SOUND::CORE::FILTER::build(Ring &ring, Whole frames) {
  ring.cells.assign(frames + SPARE, 0);
  ring.head = 0;
}

void SOUND::CORE::FILTER::push(Ring &ring, Float value) {
  ring.cells[ring.head] = value;
  ring.head = (ring.head + 1) % ring.cells.size();
}

auto SOUND::CORE::FILTER::read(const Ring &ring, Float delay) -> Float {
  const Whole whole = Whole(delay);
  const Float part = delay - Float(whole);
  const Float near = ::at(ring, whole), far = ::at(ring, whole + 1);
  return near + (far - near) * part;
}

void SOUND::CORE::FILTER::build(Comb &comb, Whole frames) {
  build(comb.ring, frames);
  comb.held = 0;
}

void SOUND::CORE::FILTER::settle(
  Comb &comb, Float delay, Float feedback, Float damp) {
  comb.delay = ::held(comb.ring, delay);
  comb.feedback = ::kept(feedback);
  comb.damp = damp < 0 ? 0 : damp > STABLE ? STABLE : damp;
}

auto SOUND::CORE::FILTER::tick(Comb &comb, Float in) -> Float {
  const Float out = read(comb.ring, comb.delay);
  comb.held = out + (comb.held - out) * comb.damp;
  push(comb.ring, in + comb.held * comb.feedback);
  return out;
}

void SOUND::CORE::FILTER::build(Allpass &allpass, Whole frames) {
  build(allpass.ring, frames);
}

void SOUND::CORE::FILTER::settle(Allpass &allpass, Float delay, Float gain) {
  allpass.delay = ::held(allpass.ring, delay);
  allpass.gain = ::kept(gain);
}

auto SOUND::CORE::FILTER::tick(Allpass &allpass, Float in) -> Float {
  const Float delayed = read(allpass.ring, allpass.delay);
  const Float fed = in + allpass.gain * delayed;
  push(allpass.ring, fed);
  return delayed - allpass.gain * fed;
}
