// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole BORROWS[CHIP::PULSES] = {1, 0};

auto shift(const CHIP::Synth &synth) -> Integer {
  return Integer(synth.rows[CHIP::SWEEP]);
}

void sweep(CHIP::Synth &synth, Whole at) {
  CHIP::Pulse &pulse = synth.pulses[at];
  if (pulse.divider == 0 && !pulse.muted && pulse.period > 0) {
    const Whole goal = CHIP::target(synth, at);
    pulse.swept *= Float(goal) / Float(pulse.period);
    pulse.period = goal;
  }
  if (pulse.divider == 0)
    pulse.divider = Whole(synth.rows[CHIP::PACE]);
  else
    --pulse.divider;
}

}  // namespace

auto SOUND::PLUGINS::CHIP::target(const Synth &synth, Whole at) -> Whole {
  const Integer size = ::shift(synth);
  const Whole period = synth.pulses[at].period;
  const Whole change = period >> Whole(size < 0 ? -size : size);
  if (size < 0) return period + change;
  const Whole taken = change + ::BORROWS[at];
  return period > taken ? period - taken : 0;
}

auto SOUND::PLUGINS::CHIP::silenced(const Synth &synth, Whole at) -> Flag {
  const Whole period = synth.pulses[at].period;
  if (period < FLOOR || period > CEILING) return true;
  return ::shift(synth) != 0 && target(synth, at) > CEILING;
}

void SOUND::PLUGINS::CHIP::arm(Synth &synth) {
  for (Pulse &pulse : synth.pulses) {
    pulse.swept = 1;
    pulse.divider = Whole(synth.rows[PACE]);
  }
}

void SOUND::PLUGINS::CHIP::sweep(Synth &synth) {
  if (::shift(synth) == 0) return;
  for (Whole at = 0; at < PULSES; ++at) ::sweep(synth, at);
}
