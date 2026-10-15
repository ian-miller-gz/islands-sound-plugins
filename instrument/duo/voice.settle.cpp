// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float UNITY = 1;

void contour(
  DUO::Contour &contour, const CORE::ENVELOPE::Envelope &envelope, Whole rate) {
  contour.envelope = envelope;
  contour.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  contour.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(contour.envelope, rate);
}

void glide(DUO::Synth &synth) {
  synth.allocator.glide.time = synth.rows[DUO::GLIDE];
  CORE::VOICE::settle(synth.allocator, synth.rate);
}

void swing(DUO::Synth &synth) {
  synth.lfo.wave = CORE::MODULATOR::SINE;
  synth.lfo.hertz = synth.rows[DUO::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
  synth.lag.time = synth.rows[DUO::LAG];
  CORE::MODULATOR::settle(synth.lag, synth.rate);
}

void drive(DUO::Synth &synth) {
  const Flag driven = synth.rows[DUO::DRIVE] > 0;
  synth.slope = Whole(synth.rows[DUO::SLOPE]);
  synth.gain = driven ? DUO::GAIN : UNITY;
  synth.saturation = driven ? UNITY : 0;
}

}  // namespace

void SOUND::PLUGINS::DUO::settle(Synth &synth) {
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
  DUO::wander(synth);
  ::drive(synth);
  CORE::FILTER::settle(
    synth.highpass, CORE::FILTER::HIGH, rows[HIGHPASS], synth.rate);
  synth.tune = rows[TUNE] / CENT;
  synth.fine = rows[FINE2] / CENT;
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
}
