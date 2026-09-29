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

auto contour(const Float *rows, Whole rate) -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::ADSR::create(
    rows[DCO::ATTACK], rows[DCO::DECAY], rows[DCO::SUSTAIN],
    rows[DCO::RELEASE]);
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

}  // namespace

void SOUND::PLUGINS::DCO::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.contour = ::contour(rows, synth.rate);
  synth.door = rows[AMPLIFIER] > 0 ? ::gate(synth.rate) : synth.contour;
  CORE::VOICE::settle(synth.allocator, synth.rate);
  ::swing(synth);
  ::pass(synth);
  settle(synth.chorus, Whole(rows[CHORUS]), synth.rate);
  synth.tune = rows[TUNE] / CENT;
  for (Voice &voice : synth.voices)
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
}
