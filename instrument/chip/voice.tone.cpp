// SPDX-License-Identifier: AGPL-3.0-or-later
#include <array>
#include <limits>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole DIGITS = std::numeric_limits<CORE::PHASE::Wheel>::digits;
constexpr Whole STEPS = 8;
constexpr Whole BITS = 3;
constexpr Whole LENGTH = 32;
constexpr Whole RAMP = 16;
constexpr Whole RAMPS = 5;
constexpr Float HALF = 0.5f;
constexpr Float GAIN = 2;

constexpr Whole DUTIES[][STEPS] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1}};

constexpr auto ramp() -> std::array<Whole, LENGTH> {
  std::array<Whole, LENGTH> values{};
  for (Whole at = 0; at < values.size(); ++at)
    values[at] = at < RAMP ? RAMP - 1 - at : at - RAMP;
  return values;
}

constexpr auto SEQUENCE = ramp();

auto square(CHIP::Pulse &pulse, Whole level) -> Whole {
  pulse.clock.phase += pulse.clock.step;
  if (pulse.muted) return 0;
  const Whole at = pulse.clock.phase >> (DIGITS - BITS);
  return DUTIES[pulse.duty < std::size(DUTIES) ? pulse.duty : 0][at] * level;
}

auto wave(CHIP::Synth &synth) -> Whole {
  CHIP::Wave &triangle = synth.triangle;
  const Flag on = synth.rows[CHIP::TRIANGLE] > HALF && synth.gated;
  if (on && !triangle.muted) triangle.clock.phase += triangle.clock.step;
  return SEQUENCE[triangle.clock.phase >> (DIGITS - RAMPS)];
}

auto hiss(CHIP::Synth &synth) -> Whole {
  CHIP::Lfsr &noise = synth.noise;
  noise.debt += noise.clocks;
  for (; noise.debt >= 1; noise.debt -= 1) CHIP::shift(noise);
  return (noise.bits & 1) != 0 ? 0 : synth.steps[CHIP::NOISE].level;
}

}  // namespace

auto SOUND::PLUGINS::CHIP::sound(Synth &synth) -> Float {
  clock(synth);
  if (synth.stale) retune(synth);
  Whole squares = 0;
  for (Whole at = 0; at < PULSES; ++at)
    squares += ::square(synth.pulses[at], synth.steps[at].level);
  Float out = mixed(squares, ::wave(synth), ::hiss(synth));
  for (CORE::FILTER::Pole &filter : synth.filters)
    out = CORE::FILTER::tick(filter, out);
  return out * ::GAIN;
}
