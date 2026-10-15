// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Whole SINGLE = 1;
constexpr Whole DECAYED = MULTIMODE::DECAY1 - MULTIMODE::ATTACK1;
constexpr Whole SUSTAINED = MULTIMODE::SUSTAIN1 - MULTIMODE::ATTACK1;
static_assert(MULTIMODE::DECAY2 - MULTIMODE::ATTACK2 == DECAYED);
static_assert(MULTIMODE::SUSTAIN2 - MULTIMODE::ATTACK2 == SUSTAINED);

auto contour(
  const Float *rows, Whole first, Float stretch, Float depth,
  Whole rate) -> CORE::ENVELOPE::Envelope {
  const Float attack = rows[first] * stretch;
  const Float decay = rows[first + DECAYED] * stretch;
  const Float sustain = rows[first + SUSTAINED];
  auto envelope = CORE::ENVELOPE::ADSR::create(attack, decay, sustain, decay);
  envelope.depth = depth;
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void shape(MULTIMODE::Synth &synth, Whole at) {
  const Float *rows = synth.rows;
  const Float stretch = MULTIMODE::stretch(synth, at);
  MULTIMODE::Voice &voice = synth.voices[at];
  voice.contour.envelope =
    ::contour(rows, MULTIMODE::ATTACK1, stretch, 0, synth.rate);
  voice.loudness.envelope = ::contour(
    rows, MULTIMODE::ATTACK2, stretch, rows[MULTIMODE::VELOCITY], synth.rate);
}

void allocate(MULTIMODE::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.glide.time = synth.rows[MULTIMODE::GLIDE];
  allocator.count = Whole(synth.rows[MULTIMODE::COUNT]);
  allocator.unison =
    synth.rows[MULTIMODE::UNISON] > 0 ? allocator.count : SINGLE;
  allocator.detune = synth.rows[MULTIMODE::DETUNE];
  CORE::VOICE::settle(allocator, synth.rate);
}

void swing(MULTIMODE::Synth &synth) {
  synth.lfo.wave = CORE::MODULATOR::TRIANGLE;
  synth.lfo.hertz = synth.rows[MULTIMODE::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::MULTIMODE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  for (Whole at = 0; at < VOICES; ++at) ::shape(synth, at);
  ::allocate(synth);
  ::swing(synth);
  wander(synth);
  synth.tune = rows[TUNE] / CENT;
  for (Voice &voice : synth.voices)
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
}
