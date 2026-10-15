// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float CLICK = 0.002f;
constexpr Float TIME = 0.1f;
constexpr Float FULL = 1;
constexpr Float WANDERING = 1;

auto contour(const Float *rows, Float scale, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::ADSR::create(
    scale * rows[DCO::ATTACK], scale * rows[DCO::DECAY], rows[DCO::SUSTAIN],
    scale * rows[DCO::RELEASE]);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

auto gate(Whole rate) -> CORE::ENVELOPE::Envelope {
  auto envelope =
    CORE::ENVELOPE::ADSR::create(CLICK, 0, CORE::ENVELOPE::FULL, CLICK);
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void swing(DCO::Synth &synth) {
  synth.lfo.wave = CORE::MODULATOR::TRIANGLE;
  synth.lfo.hertz = synth.rows[DCO::RATE];
  synth.lfo.fade = synth.rows[DCO::DELAY];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

void shape(DCO::Synth &synth) {
  const Float *rows = synth.rows;
  const CORE::ENVELOPE::Envelope gated = ::gate(synth.rate);
  for (Whole at = 0; at < DCO::VOICES; ++at) {
    DCO::Voice &voice = synth.voices[at];
    const Float scale = FULL + TIME * rows[DCO::TRIM] * synth.trims[at].time;
    voice.filter.shape = ::contour(rows, scale, synth.rate);
    voice.amplifier.shape =
      rows[DCO::AMPLIFIER] > 0 ? gated : voice.filter.shape;
    voice.amplifier.shape.depth = rows[DCO::VELOCITY];
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
    CORE::MODULATOR::settle(voice.drift, WANDERING, synth.rate);
  }
}

}  // namespace

void SOUND::PLUGINS::DCO::settle(Synth &synth) {
  const Float *rows = synth.rows;
  ::shape(synth);
  assign(synth);
  ::swing(synth);
  settle(synth.highpass, Whole(rows[HIGHPASS]), synth.rate);
  settle(synth.chorus, Whole(rows[CHORUS]), synth.rate);
  synth.tune = rows[TUNE] / CENT;
}
