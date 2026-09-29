// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto pitched(Float hertz) -> Float {
  return CORE::PHASE::ANCHOR +
         CORE::PHASE::OCTAVE * std::log2(hertz / CORE::PHASE::CONCERT);
}

auto base(const HARMONIZER::Harmonizer &harmonizer) -> Float {
  const Whole mode = Whole(harmonizer.rows[HARMONIZER::MODE]);
  return mode == HARMONIZER::ABSOLUTE ? harmonizer.pitch
                                      : harmonizer.rows[HARMONIZER::KEY];
}

auto bounded(Float cents) -> Float {
  return cents < -HARMONIZER::RANGE  ? -HARMONIZER::RANGE
         : cents > HARMONIZER::RANGE ? HARMONIZER::RANGE
                                     : cents;
}

void steer(
  HARMONIZER::Harmonizer &harmonizer, HARMONIZER::Part &part,
  CORE::VOICE::Note &note) {
  const Float interval = Float(note.pitch) - ::base(harmonizer);
  const Float target = ::bounded(interval * HARMONIZER::CENT);
  if (part.fresh) CORE::MODULATOR::jump(part.glide, target);
  part.fresh = false;
  const Float cents = CORE::MODULATOR::tick(part.glide, target);
  const Float seconds = HARMONIZER::WINDOW;
  if (harmonizer.keep)
    CORE::PITCH::settle(part.formant, cents, 0, seconds, harmonizer.rate);
  else
    CORE::PITCH::settle(part.grains, cents, seconds, harmonizer.rate);
  if (!note.held && part.gate.value < HARMONIZER::QUIET) note.sounding = false;
}

}  // namespace

void SOUND::PLUGINS::HARMONIZER::follow(Harmonizer &harmonizer, Float sample) {
  if (!CORE::PITCH::feed(harmonizer.detector, &sample, 1)) return;
  const CORE::PITCH::Estimate &estimate = harmonizer.detector.estimate;
  if (estimate.confidence < VOICED || estimate.hertz <= 0) return;
  harmonizer.pitch = ::pitched(estimate.hertz);
}

void SOUND::PLUGINS::HARMONIZER::steer(Harmonizer &harmonizer) {
  for (Whole at = 0; at < VOICES; ++at) {
    CORE::VOICE::Note &note = harmonizer.allocator.notes[at];
    if (note.sounding) ::steer(harmonizer, harmonizer.parts[at], note);
  }
}
