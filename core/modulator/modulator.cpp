// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SINGLE = 1.0f;
constexpr Float NYQUIST = 2.0f;
constexpr Whole LEFT = 13;
constexpr Whole RIGHT = 17;
constexpr Whole AGAIN = 5;
constexpr Float SPREAD = 2.0f;

}  // namespace

auto SOUND::PLUGINS::CORE::MODULATOR::delta(Float seconds, Whole rate)
  -> Float {
  const Float frames = seconds * Float(rate);
  return frames <= ::SINGLE ? FULL : FULL / frames;
}

auto SOUND::PLUGINS::CORE::MODULATOR::pole(Float seconds, Whole rate) -> Float {
  const Float frames = seconds * Float(rate);
  return frames <= ::SINGLE ? FULL : FULL - std::exp(-FULL / frames);
}

auto SOUND::PLUGINS::CORE::MODULATOR::draw(Seed &seed) -> Float {
  if (seed == 0) seed = SEED;
  seed ^= seed << ::LEFT;
  seed ^= seed >> ::RIGHT;
  seed ^= seed << ::AGAIN;
  return PHASE::fraction(seed) * ::SPREAD - FULL;
}

void SOUND::PLUGINS::CORE::MODULATOR::settle(Lfo &lfo, Whole rate) {
  const Float period = lfo.period < ::NYQUIST ? ::NYQUIST : lfo.period;
  lfo.step = lfo.period > 0 ? PHASE::Wheel(PHASE::TURN / period)
                            : PHASE::step(lfo.hertz, rate);
  lfo.rise = delta(lfo.fade, rate);
  lfo.glide = pole(lfo.slew, rate);
}

void SOUND::PLUGINS::CORE::MODULATOR::settle(Slew &slew, Whole rate) {
  slew.up = delta(slew.rise, rate);
  slew.down = delta(slew.fall, rate);
}

void SOUND::PLUGINS::CORE::MODULATOR::jump(Slew &slew, Float value) {
  slew.value = value;
}

auto SOUND::PLUGINS::CORE::MODULATOR::tick(Slew &slew, Float in) -> Float {
  const Float change = in - slew.value;
  slew.value += change > slew.up      ? slew.up
                : change < -slew.down ? -slew.down
                                      : change;
  return slew.value;
}

void SOUND::PLUGINS::CORE::MODULATOR::settle(Smoother &smoother, Whole rate) {
  smoother.pole = pole(smoother.time, rate);
}

void SOUND::PLUGINS::CORE::MODULATOR::jump(Smoother &smoother, Float value) {
  smoother.value = value;
}

auto SOUND::PLUGINS::CORE::MODULATOR::tick(Smoother &smoother, Float target)
  -> Float {
  smoother.value += (target - smoother.value) * smoother.pole;
  return smoother.value;
}
