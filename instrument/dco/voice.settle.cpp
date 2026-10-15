// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float CLICK = 0.002f;
constexpr Float SHELF = 100;
constexpr Float BOOST = 6;
constexpr Float LOWER = 240;
constexpr Float UPPER = 720;
constexpr Whole FLATTENED = 1;
constexpr Float TIME = 0.1f;
constexpr Float FULL = 1;
constexpr Float WANDERING = 1;

struct Position {
  Whole kind;
  Float cutoff;
  Float gain;
};

constexpr Position POSITIONS[] = {
  {CORE::FILTER::LOWSHELF, SHELF, BOOST},
  {CORE::FILTER::LOWSHELF, SHELF, 0},
  {CORE::FILTER::HIGH, LOWER, 0},
  {CORE::FILTER::HIGH, UPPER, 0}};
static_assert(std::size(POSITIONS) == std::size(DCO::POSITIONS));

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

void pass(DCO::Synth &synth) {
  const Whole at = Whole(synth.rows[DCO::HIGHPASS]);
  const Position &position =
    ::POSITIONS[at < std::size(::POSITIONS) ? at : FLATTENED];
  CORE::FILTER::settle(
    synth.highpass, position.kind, position.cutoff, CORE::FILTER::FLAT,
    position.gain, synth.rate);
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
  ::pass(synth);
  settle(synth.chorus, Whole(rows[CHORUS]), synth.rate);
  synth.tune = rows[TUNE] / CENT;
}
