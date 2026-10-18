// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float DECIBELS = 6.02f;
constexpr Whole PARITY = 2;
constexpr Float FULL = 1;
constexpr Float CENT = 100;

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

auto balanced(Float balance, Whole order) -> Float {
  if (order == 1) return FULL;
  return order % PARITY == 0 ? FULL + std::min(balance, 0.0f)
                             : FULL - std::max(balance, 0.0f);
}

void spread(ADDITIVE::Synth &synth) {
  const Float *rows = synth.rows;
  const Float tilt = rows[ADDITIVE::TILT] / DECIBELS;
  const Float stretch = FULL + rows[ADDITIVE::STRETCH] / CORE::PHASE::CENTS;
  for (Whole at = 0; at < ADDITIVE::BANK; ++at) {
    const Float order = Float(at + 1);
    synth.weights[at] = ::series(rows, at) * std::pow(order, tilt) *
                        ::balanced(rows[ADDITIVE::BALANCE], at + 1);
    synth.ratios[at] = std::pow(order, stretch);
  }
}

void swing(ADDITIVE::Synth &synth) {
  synth.lfo.wave = Whole(synth.rows[ADDITIVE::WAVE]);
  synth.lfo.hertz = synth.rows[ADDITIVE::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

void glide(ADDITIVE::Synth &synth) {
  synth.allocator.glide.time = synth.rows[ADDITIVE::GLIDE];
  synth.allocator.glide.slide = CORE::VOICE::ALWAYS;
  CORE::VOICE::settle(synth.allocator, synth.rate);
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
  ::swing(synth);
  ::glide(synth);
  synth.tune = rows[TUNE] / ::CENT;
}
