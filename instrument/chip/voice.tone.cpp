// SPDX-License-Identifier: AGPL-3.0-or-later
#include <array>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole HALVES = 2;
constexpr Whole RAMP = CHIP::CHANNEL::RAMP / HALVES;
constexpr Whole BIT = 1;
constexpr Float GAIN = 2;

constexpr Whole PATTERNS[][CHIP::CHANNEL::SQUARE] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1}};

constexpr auto ramp() -> std::array<Whole, CHIP::CHANNEL::RAMP> {
  std::array<Whole, CHIP::CHANNEL::RAMP> values{};
  for (Whole at = 0; at < values.size(); ++at)
    values[at] = at < RAMP ? at : CHIP::CHANNEL::RAMP - 1 - at;
  return values;
}

constexpr auto SEQUENCE = ramp();

auto square(const CHIP::Pulse &pulse, Whole level) -> Whole {
  if (pulse.muted) return 0;
  const Whole duty = pulse.duty < std::size(PATTERNS) ? pulse.duty : 0;
  return PATTERNS[duty][pulse.position] * level;
}

}  // namespace

void SOUND::PLUGINS::CHIP::advance(Pulse &pulse) {
  pulse.position = (pulse.position + 1) % CHANNEL::SQUARE;
}

void SOUND::PLUGINS::CHIP::advance(Wave &triangle) {
  triangle.position = (triangle.position + 1) % CHANNEL::RAMP;
}

auto SOUND::PLUGINS::CHIP::level(const Synth &synth) -> Float {
  Whole squares = 0;
  for (Whole at = 0; at < PULSES; ++at)
    squares += ::square(synth.pulses[at], synth.steps[at].level);
  const Whole wave = ::SEQUENCE[synth.triangle.position];
  const Flag quiet = (synth.noise.bits & ::BIT) != 0;
  return mixed(squares, wave, quiet ? 0 : synth.steps[NOISE].level);
}

auto SOUND::PLUGINS::CHIP::sound(Synth &synth) -> Float {
  clock(synth);
  if (synth.stale) retune(synth);
  const Float now = level(synth);
  CORE::OSCILLATOR::jump(synth.edges, now - synth.edges.level, 0);
  Float out = CORE::OSCILLATOR::tick(synth.edges, run(synth, now));
  for (CORE::FILTER::Pole &filter : synth.filters)
    out = CORE::FILTER::tick(filter, out);
  return out * ::GAIN;
}
