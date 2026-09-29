// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "reverb.hpp"

namespace {
using namespace SOUND;

constexpr Float SILENCE = 0.001f;

}  // namespace

auto SOUND::CORE::REVERB::decay(Float frames, Float seconds, Whole rate)
  -> Float {
  if (seconds <= 0 || rate == 0) return 0;
  return std::pow(SILENCE, frames / (seconds * Float(rate)));
}

auto SOUND::CORE::REVERB::clamped(Float size) -> Float {
  return size < SMALLEST ? SMALLEST : size > LARGEST ? LARGEST : size;
}

void SOUND::CORE::REVERB::build(Delay &delay, Whole frames) {
  LINE::build(delay.line, frames);
}

void SOUND::CORE::REVERB::settle(Delay &delay, Float frames) {
  delay.delay = frames;
}

auto SOUND::CORE::REVERB::tick(Delay &delay, Float in) -> Float {
  return LINE::tick(delay.line, in, delay.delay);
}

void SOUND::CORE::REVERB::build(Comb &comb, Whole frames) {
  LINE::build(comb.line, frames);
}

void SOUND::CORE::REVERB::settle(
  Comb &comb, Float frames, Float seconds, Float cutoff, Whole rate) {
  comb.delay = frames;
  LINE::settle(comb.loop, decay(frames, seconds, rate), cutoff, rate);
}

auto SOUND::CORE::REVERB::tick(Comb &comb, Float in) -> Float {
  return LINE::tick(comb.line, comb.loop, in, comb.delay);
}

void SOUND::CORE::REVERB::build(Allpass &allpass, Whole frames) {
  LINE::build(allpass.line, frames);
}

void SOUND::CORE::REVERB::settle(Allpass &allpass, Float frames, Float gain) {
  allpass.delay = frames;
  allpass.gain = gain;
}

auto SOUND::CORE::REVERB::tick(Allpass &allpass, Float in) -> Float {
  return tick(allpass, in, 0);
}

auto SOUND::CORE::REVERB::tick(Allpass &allpass, Float in, Float offset)
  -> Float {
  const Float delayed = LINE::read(allpass.line, allpass.delay + offset);
  const Float fed = in + allpass.gain * delayed;
  LINE::write(allpass.line, fed);
  return delayed - allpass.gain * fed;
}
