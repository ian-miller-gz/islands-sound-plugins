// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Whole SINGLE = 1;

auto contour(Float attack, Float decay, Float sustain, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::ADSR::create(attack, decay, sustain, decay);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void allocate(MULTIMODE::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.glide.time = synth.rows[MULTIMODE::GLIDE];
  allocator.unison =
    synth.rows[MULTIMODE::UNISON] > 0 ? MULTIMODE::VOICES : SINGLE;
  allocator.detune = synth.rows[MULTIMODE::DETUNE];
  CORE::VOICE::settle(allocator, synth.rate);
}

void swing(MULTIMODE::Synth &synth) {
  synth.lfo.wave = CORE::MODULATOR::TRIANGLE;
  synth.lfo.hertz = synth.rows[MULTIMODE::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::MULTIMODE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.contour =
    ::contour(rows[ATTACK1], rows[DECAY1], rows[SUSTAIN1], synth.rate);
  synth.loudness =
    ::contour(rows[ATTACK2], rows[DECAY2], rows[SUSTAIN2], synth.rate);
  ::allocate(synth);
  ::swing(synth);
  synth.tune = rows[TUNE] / CENT;
  for (Voice &voice : synth.voices)
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
}
