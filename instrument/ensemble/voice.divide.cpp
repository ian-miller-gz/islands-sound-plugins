// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

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

auto paraphonic(ENSEMBLE::Synth &synth, ENSEMBLE::Key &key, Float swell)
  -> Float {
  if (!key.held) CORE::ENVELOPE::choke(key.gate);
  return ::ramped(key, synth.ramp) * swell;
}

auto polyphonic(ENSEMBLE::Synth &synth, ENSEMBLE::Key &key, Float) -> Float {
  key.level = 0;
  return CORE::ENVELOPE::tick(key.gate, synth.envelope);
}

using Read = auto (*)(ENSEMBLE::Synth &, ENSEMBLE::Key &, Float) -> Float;

constexpr Read READS[] = {paraphonic, polyphonic};
static_assert(std::size(READS) == ENSEMBLE::MODES + 1);

auto live(const ENSEMBLE::Key &key) -> Flag {
  return key.target > 0 || key.level > 0 || CORE::ENVELOPE::sounding(key.gate);
}

void advance(ENSEMBLE::Synth &synth) {
  for (Whole at = 0; at < ENSEMBLE::CLASSES; ++at)
    synth.counters[at] += synth.steps[at];
}

}  // namespace

auto SOUND::PLUGINS::ENSEMBLE::divide(Synth &synth, Float swell) -> Sums {
  Sums sums;
  const Flag silent = synth.held == 0 && !CORE::ENVELOPE::sounding(synth.gate);
  const ::Read read = ::READS[Whole(synth.rows[ENVELOPE])];
  const Float depth = synth.rows[VELOCITY];
  for (Whole at = 0; !silent && at < CORE::PHASE::PITCHES; ++at) {
    Key &key = synth.keys[at];
    if (!::live(key)) continue;
    const Float level =
      read(synth, key, swell) * CORE::ENVELOPE::weigh(key.velocity, depth);
    if (level <= 0) continue;
    const Whole shift = (TOP + at % OCTAVE - at) / OCTAVE;
    sums.eight += level * ::saw(synth, at, shift);
    sums.sixteen += level * ::saw(synth, at, shift + LOWER);
  }
  ::advance(synth);
  return sums;
}
