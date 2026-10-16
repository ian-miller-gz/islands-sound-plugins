// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto envelope(
  Float attack, Float decay, Float sustain, Float release, Whole curve,
  Whole rate) -> CORE::ENVELOPE::Envelope {
  auto shaped = CORE::ENVELOPE::ADSR::create(attack, decay, sustain, release);
  shaped.curve = curve;
  shaped.trigger = CORE::ENVELOPE::RESET;
  CORE::ENVELOPE::shape(shaped, rate);
  return shaped;
}

auto series(const Float *rows, Whole at) -> Float {
  if (at < ADDITIVE::HARMONICS) return rows[ADDITIVE::HARMONIC1 + at];
  return rows[ADDITIVE::HARMONIC8] * Float(ADDITIVE::HARMONICS) / Float(at + 1);
}

void spread(ADDITIVE::Synth &synth) {
  for (Whole at = 0; at < ADDITIVE::BANK; ++at) {
    synth.weights[at] = ::series(synth.rows, at);
    synth.ratios[at] = Float(at + 1);
  }
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.door = ::envelope(
    rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE],
    CORE::ENVELOPE::LINEAR, synth.rate);
  synth.door.depth = rows[VELOCITY];
  synth.contour = ::envelope(
    rows[SWEEP], rows[CLOSE], 0, rows[CLOSE], CORE::ENVELOPE::EXPONENTIAL,
    synth.rate);
  ::spread(synth);
}
