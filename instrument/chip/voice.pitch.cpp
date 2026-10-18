// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float PULSING = 16;
constexpr Float WAVING = 32;
constexpr Whole LOWEST = 2;
constexpr Float HALF = 0.5f;

auto counted(Float pitch, Float divider, Float scale) -> Whole {
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float period = std::round((CHIP::CPU / (divider * hertz) - 1) * scale);
  return period < 0 ? 0 : Whole(period);
}

auto driven(Float divider, Whole length, Whole period, Whole rate) -> Float {
  const Float hertz = CHIP::CPU / (divider * Float(period + 1));
  return hertz * Float(length) / Float(rate);
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
    synth.timers[at].clocks =
      ::driven(::PULSING, CHANNEL::SQUARE, pulse.period, synth.rate);
  }
  Wave &triangle = synth.triangle;
  const Whole period = ::counted(base + synth.rows[OFFSET3], ::WAVING, 1);
  triangle.muted = period < ::LOWEST || period > CEILING;
  const Flag on = synth.rows[TRIANGLE] > ::HALF && synth.gated;
  synth.timers[CHANNEL::WAVE].clocks =
    on && !triangle.muted
      ? ::driven(::WAVING, CHANNEL::RAMP, period, synth.rate)
      : 0;
  synth.stale = false;
}
