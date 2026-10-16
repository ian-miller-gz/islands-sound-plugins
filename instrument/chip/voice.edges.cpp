// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNIT = 1;
constexpr Float NEVER = 2;
constexpr Whole LIMIT = 64;

using Cross = void (*)(CHIP::Synth &, Whole);

void square(CHIP::Synth &synth, Whole at) { CHIP::advance(synth.pulses[at]); }

void ramp(CHIP::Synth &synth, Whole) { CHIP::advance(synth.triangle); }

void hiss(CHIP::Synth &synth, Whole) { CHIP::shift(synth.noise); }

constexpr Cross CROSSES[] = {square, square, ramp, hiss};
static_assert(std::size(CROSSES) == CHIP::CHANNEL::COUNT);

struct Edge {
  Whole channel = CHIP::CHANNEL::COUNT;
  Float time = UNIT;
};

auto due(const CHIP::Timer &timer) -> Float {
  return timer.clocks > 0 ? (UNIT - timer.debt) / timer.clocks : NEVER;
}

auto soonest(const CHIP::Synth &synth) -> Edge {
  Edge edge;
  for (Whole at = 0; at < CHIP::CHANNEL::COUNT; ++at) {
    const Float time = due(synth.timers[at]);
    if (time <= edge.time) edge = {at, time};
  }
  return edge;
}

}  // namespace

auto SOUND::PLUGINS::CHIP::run(Synth &synth, Float before) -> Float {
  for (Whole count = 0; count < ::LIMIT; ++count) {
    const ::Edge edge = ::soonest(synth);
    if (edge.channel >= CHANNEL::COUNT) break;
    ::CROSSES[edge.channel](synth, edge.channel);
    synth.timers[edge.channel].debt -= ::UNIT;
    const Float after = level(synth);
    CORE::OSCILLATOR::jump(synth.edges, after - before, edge.time);
    before = after;
  }
  for (Timer &timer : synth.timers)
    timer.debt = std::fmod(timer.debt + timer.clocks, ::UNIT);
  return before;
}
