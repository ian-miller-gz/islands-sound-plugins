// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;

void contour(
  DUO::Contour &contour, const CORE::ENVELOPE::Envelope &envelope, Whole rate) {
  contour.envelope = envelope;
  contour.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  contour.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(contour.envelope, rate);
}

void glide(DUO::Synth &synth) {
  for (CORE::VOICE::Allocator *allocator : {&synth.low, &synth.high}) {
    allocator->glide.time = synth.rows[DUO::GLIDE];
    CORE::VOICE::settle(*allocator, synth.rate);
  }
}

void swing(DUO::Synth &synth) {
  synth.lfo.wave = CORE::MODULATOR::SINE;
  synth.lfo.hertz = synth.rows[DUO::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
  synth.lag.time = synth.rows[DUO::LAG];
  CORE::MODULATOR::settle(synth.lag, synth.rate);
}

}  // namespace

void SOUND::DUO::settle(Synth &synth) {
  const Float *rows = synth.rows;
  ::contour(
    synth.adsr,
    CORE::ENVELOPE::ADSR::create(
      rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]),
    synth.rate);
  ::contour(
    synth.ar, CORE::ENVELOPE::AR::create(rows[ATTACK2], rows[RELEASE2]),
    synth.rate);
  ::glide(synth);
  ::swing(synth);
  CORE::FILTER::settle(
    synth.highpass, CORE::FILTER::HIGH, rows[HIGHPASS], synth.rate);
  synth.tune = rows[TUNE] / CENT;
  synth.fine = rows[FINE2] / CENT;
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
}
