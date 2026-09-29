// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;

struct Stages {
  Whole attack;
  Whole decay;
  Whole sustain;
  Whole release;
};

constexpr Stages CONTOUR = {
  SUPERSAW::ATTACK1, SUPERSAW::DECAY1, SUPERSAW::SUSTAIN1, SUPERSAW::RELEASE1};
constexpr Stages LOUDNESS = {
  SUPERSAW::ATTACK2, SUPERSAW::DECAY2, SUPERSAW::SUSTAIN2, SUPERSAW::RELEASE2};

auto contour(const Float *rows, const Stages &stages, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::ADSR::create(
    rows[stages.attack], rows[stages.decay], rows[stages.sustain],
    rows[stages.release]);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void stack(SUPERSAW::Synth &synth) {
  const Float *rows = synth.rows;
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.unison = Whole(rows[SUPERSAW::UNISON]);
  allocator.detune = rows[SUPERSAW::SPREAD];
  allocator.spread = rows[SUPERSAW::WIDTH];
  allocator.glide.time = rows[SUPERSAW::GLIDE];
  allocator.glide.slide = CORE::VOICE::ALWAYS;
  CORE::VOICE::settle(allocator, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::SUPERSAW::settle(Synth &synth) {
  synth.contour = ::contour(synth.rows, ::CONTOUR, synth.rate);
  synth.door = ::contour(synth.rows, ::LOUDNESS, synth.rate);
  ::stack(synth);
  synth.tune = synth.rows[TUNE] / CENT;
  for (Voice &voice : synth.voices) voice.stale = true;
}
