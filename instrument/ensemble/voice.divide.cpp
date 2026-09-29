// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole OCTAVE = 12;
constexpr Whole LOWER = 1;
constexpr ENSEMBLE::Counter NYQUIST = ENSEMBLE::Counter(1) << 31;

auto saw(const ENSEMBLE::Synth &synth, Whole key, Whole shift) -> Float {
  const Whole pitch = key % OCTAVE;
  const ENSEMBLE::Counter step = synth.steps[pitch] >> shift;
  if (step >= NYQUIST) return 0;
  const CORE::OSCILLATOR::Oscillator divided = {
    .phase = CORE::PHASE::Wheel(synth.counters[pitch] >> shift),
    .step = CORE::PHASE::Wheel(step)};
  return CORE::OSCILLATOR::saw(divided);
}

auto ramped(ENSEMBLE::Key &key, Float ramp) -> Float {
  const Float change = key.target - key.level;
  key.level += change > ramp ? ramp : change < -ramp ? -ramp : change;
  return key.level;
}

void advance(ENSEMBLE::Synth &synth) {
  for (Whole at = 0; at < ENSEMBLE::CLASSES; ++at)
    synth.counters[at] += synth.steps[at];
}

}  // namespace

auto SOUND::ENSEMBLE::divide(Synth &synth) -> Sums {
  Sums sums;
  const Flag silent = synth.held == 0 && !CORE::ENVELOPE::sounding(synth.gate);
  for (Whole at = 0; !silent && at < CORE::PHASE::PITCHES; ++at) {
    Key &key = synth.keys[at];
    if (key.level <= 0 && key.target <= 0) continue;
    const Float level = ::ramped(key, synth.ramp);
    const Whole shift = (TOP + at % OCTAVE - at) / OCTAVE;
    sums.eight += level * ::saw(synth, at, shift);
    sums.sixteen += level * ::saw(synth, at, shift + LOWER);
  }
  ::advance(synth);
  return sums;
}
