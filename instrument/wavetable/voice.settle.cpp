// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float FULL = 1;

struct Stages {
  Whole attack;
  Whole decay;
  Whole sustain;
  Whole release;
};

constexpr Stages CONTOUR = {
  WAVETABLE::ATTACK1, WAVETABLE::DECAY1, WAVETABLE::SUSTAIN1,
  WAVETABLE::RELEASE1};
constexpr Stages LOUDNESS = {
  WAVETABLE::ATTACK2, WAVETABLE::DECAY2, WAVETABLE::SUSTAIN2,
  WAVETABLE::RELEASE2};

auto contour(const Float *rows, const Stages &stages, Float depth, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::ADSR::create(
    rows[stages.attack], rows[stages.decay], rows[stages.sustain],
    rows[stages.release]);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  envelope.depth = depth;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void stack(WAVETABLE::Synth &synth) {
  const Float *rows = synth.rows;
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.unison = Whole(rows[WAVETABLE::UNISON]);
  allocator.detune = rows[WAVETABLE::SPREAD];
  allocator.spread = rows[WAVETABLE::WIDTH];
  allocator.glide.time = rows[WAVETABLE::GLIDE];
  allocator.glide.slide = CORE::VOICE::ALWAYS;
  CORE::VOICE::settle(allocator, synth.rate);
  synth.weight = FULL / std::sqrt(Float(CORE::VOICE::stacked(allocator)));
}

void swing(WAVETABLE::Synth &synth) {
  synth.lfo.wave = Whole(synth.rows[WAVETABLE::WAVE]);
  synth.lfo.hertz = synth.rows[WAVETABLE::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::WAVETABLE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.contour = ::contour(rows, ::CONTOUR, 0, synth.rate);
  synth.door = ::contour(rows, ::LOUDNESS, rows[VELOCITY], synth.rate);
  ::stack(synth);
  ::swing(synth);
  synth.warp = Whole(rows[WARP]);
  synth.slope = WAVETABLE::slope(synth.warp, rows[AMOUNT]);
  synth.tune = rows[TUNE] / CENT;
  for (Voice &voice : synth.voices) CORE::BLOCK::reset(voice.stride);
}
