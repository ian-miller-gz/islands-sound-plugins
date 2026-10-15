// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float SWAY = 0.5f;
constexpr Float DEPTH = 2.5f;
constexpr Float MILLISECOND = 0.001f;

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

constexpr Whole SHAPES[] = {
  CORE::MODULATOR::SINE, CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SAW,
  CORE::MODULATOR::SQUARE};
static_assert(std::size(SHAPES) == std::size(SUPERSAW::WAVES));
constexpr Whole FIRST = 0;

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

void swing(SUPERSAW::Synth &synth) {
  const Whole wave = Whole(synth.rows[SUPERSAW::WAVE]);
  synth.lfo.wave = SHAPES[wave < std::size(SHAPES) ? wave : FIRST];
  synth.lfo.hertz = synth.rows[SUPERSAW::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::SUPERSAW::settle(Synth &synth) {
  synth.contour = ::contour(synth.rows, ::CONTOUR, synth.rate);
  synth.door = ::contour(synth.rows, ::LOUDNESS, synth.rate);
  synth.door.depth = synth.rows[VELOCITY];
  ::stack(synth);
  ::swing(synth);
  CORE::LINE::settle(
    synth.chorus, ::SWAY, ::DEPTH * ::MILLISECOND, synth.rows[CHORUS],
    synth.rate);
  synth.tune = synth.rows[TUNE] / CENT;
  for (Voice &voice : synth.voices) CORE::BLOCK::reset(voice.stride);
}
