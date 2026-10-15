// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float CUT = 0.005f;
constexpr Float TIME = 0.1f;
constexpr Float FULL = 1;
constexpr Float WANDERING = 1;
constexpr Float PACE = 0.5f;
constexpr Float REACH = 0.0025f;
constexpr Whole SINGLE = 1;
constexpr Whole DECAY = POLYMOD::DECAY1 - POLYMOD::ATTACK1;
constexpr Whole SUSTAIN = POLYMOD::SUSTAIN1 - POLYMOD::ATTACK1;
constexpr Whole RELEASE = POLYMOD::RELEASE1 - POLYMOD::ATTACK1;

constexpr Whole SHAPES[] = {
  CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SAW, CORE::MODULATOR::SQUARE};
static_assert(std::size(SHAPES) == std::size(POLYMOD::WAVES));

auto contour(const Float *stages, Flag released, Float scale, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  const Float release = released ? scale * stages[RELEASE] : CUT;
  auto envelope = CORE::ENVELOPE::ADSR::create(
    scale * *stages, scale * stages[DECAY], stages[SUSTAIN], release);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void shape(POLYMOD::Synth &synth) {
  const Float *rows = synth.rows;
  const Flag released = rows[POLYMOD::RELEASE] > 0;
  for (Whole at = 0; at < POLYMOD::VOICES; ++at) {
    POLYMOD::Voice &voice = synth.voices[at];
    const Float trim = rows[POLYMOD::TRIM] * synth.trims[at].time;
    const Float scale = FULL + TIME * trim;
    voice.contour.shape =
      ::contour(rows + POLYMOD::ATTACK1, released, scale, synth.rate);
    voice.loudness.shape =
      ::contour(rows + POLYMOD::ATTACK2, released, scale, synth.rate);
    voice.loudness.shape.depth = rows[POLYMOD::VELOCITY];
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
    CORE::MODULATOR::settle(voice.drift, WANDERING, synth.rate);
  }
}

void allocate(POLYMOD::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.glide.time = synth.rows[POLYMOD::GLIDE];
  allocator.unison = synth.rows[POLYMOD::UNISON] > 0 ? POLYMOD::VOICES : SINGLE;
  allocator.detune = synth.rows[POLYMOD::DETUNE];
  CORE::VOICE::settle(allocator, synth.rate);
}

void swing(POLYMOD::Synth &synth) {
  const Whole wave = Whole(synth.rows[POLYMOD::WAVE]);
  synth.lfo.wave = ::SHAPES[wave < std::size(::SHAPES) ? wave : 0];
  synth.lfo.hertz = synth.rows[POLYMOD::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::POLYMOD::settle(Synth &synth) {
  ::shape(synth);
  ::allocate(synth);
  ::swing(synth);
  synth.tune = synth.rows[TUNE] / CENT;
  CORE::LINE::settle(
    synth.chorus, ::PACE, ::REACH, synth.rows[CHORUS], synth.rate);
}
