// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole MOST = 2047;

struct Timer {
  Float divider;
  Whole least;
};

constexpr Timer PULSING = {16, 8};
constexpr Timer WAVING = {32, 2};

auto divide(CHIP::Clock &clock, const Timer &timer, Float pitch, Whole rate)
  -> Flag {
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float counted = std::round(CHIP::CPU / (timer.divider * hertz)) - 1;
  const Whole period = counted < 0 ? 0 : Whole(counted);
  const Float actual = CHIP::CPU / (timer.divider * Float(period + 1));
  clock.step = CORE::PHASE::step(actual, rate);
  return period < timer.least || period > MOST;
}

}  // namespace

void SOUND::PLUGINS::CHIP::retune(Synth &synth) {
  const Float *rows = synth.rows;
  const Float offsets[POSITIONS] = {0, rows[FIRST], rows[SECOND]};
  const Float base = Float(synth.allocator.notes[0].pitch) + synth.tune +
                     rows[BEND] + offsets[synth.position % POSITIONS];
  const Float pitches[PULSES] = {base, base + rows[OFFSET2]};
  for (Whole at = 0; at < PULSES; ++at)
    synth.pulses[at].muted =
      ::divide(synth.pulses[at].clock, ::PULSING, pitches[at], synth.rate);
  synth.triangle.muted =
    ::divide(synth.triangle.clock, ::WAVING, base + rows[OFFSET3], synth.rate);
  synth.stale = false;
}
