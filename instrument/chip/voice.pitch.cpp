// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float PULSING = 16;
constexpr Float WAVING = 32;
constexpr Whole LOWEST = 2;

auto counted(Float pitch, Float divider, Float scale) -> Whole {
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float period = std::round((CHIP::CPU / (divider * hertz) - 1) * scale);
  return period < 0 ? 0 : Whole(period);
}

void drive(CHIP::Clock &clock, Float divider, Whole period, Whole rate) {
  const Float hertz = CHIP::CPU / (divider * Float(period + 1));
  clock.step = CORE::PHASE::step(hertz, rate);
}

auto pitched(const CHIP::Synth &synth) -> Float {
  const Float *rows = synth.rows;
  const Float offsets[CHIP::POSITIONS] = {
    0, rows[CHIP::FIRST], rows[CHIP::SECOND]};
  return synth.allocator.notes[0].current + synth.tune + rows[CHIP::BEND] +
         offsets[synth.position % CHIP::POSITIONS] + synth.wobble;
}

}  // namespace

void SOUND::PLUGINS::CHIP::retune(Synth &synth) {
  const Float base = ::pitched(synth);
  const Float pitches[PULSES] = {base, base + synth.rows[OFFSET2]};
  for (Whole at = 0; at < PULSES; ++at) {
    Pulse &pulse = synth.pulses[at];
    pulse.period = ::counted(pitches[at], ::PULSING, pulse.swept);
    pulse.muted = silenced(synth, at);
    ::drive(pulse.clock, ::PULSING, pulse.period, synth.rate);
  }
  Wave &triangle = synth.triangle;
  const Whole period = ::counted(base + synth.rows[OFFSET3], ::WAVING, 1);
  triangle.muted = period < ::LOWEST || period > CEILING;
  ::drive(triangle.clock, ::WAVING, period, synth.rate);
  synth.stale = false;
}
