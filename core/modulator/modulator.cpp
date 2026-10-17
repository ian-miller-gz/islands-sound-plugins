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
  const Float skew = lfo.skew < 0 ? 0 : lfo.skew > FULL ? FULL : lfo.skew;
  lfo.split = Whole(skew * PHASE::TURN);
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
