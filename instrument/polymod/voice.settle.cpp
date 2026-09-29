// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float CUT = 0.005f;
constexpr Whole SINGLE = 1;
constexpr Whole DECAY = POLYMOD::DECAY1 - POLYMOD::ATTACK1;
constexpr Whole SUSTAIN = POLYMOD::SUSTAIN1 - POLYMOD::ATTACK1;
constexpr Whole RELEASE = POLYMOD::RELEASE1 - POLYMOD::ATTACK1;

constexpr Whole SHAPES[] = {
  CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SAW, CORE::MODULATOR::SQUARE};
static_assert(std::size(SHAPES) == std::size(POLYMOD::WAVES));

auto contour(const Float *stages, Flag released, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  const Float release = released ? stages[RELEASE] : CUT;
  auto envelope = CORE::ENVELOPE::ADSR::create(
    *stages, stages[DECAY], stages[SUSTAIN], release);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
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

void SOUND::POLYMOD::settle(Synth &synth) {
  const Float *rows = synth.rows;
  const Flag released = rows[RELEASE] > 0;
  synth.contour = ::contour(rows + ATTACK1, released, synth.rate);
  synth.loudness = ::contour(rows + ATTACK2, released, synth.rate);
  ::allocate(synth);
  ::swing(synth);
  synth.tune = rows[TUNE] / CENT;
  for (Voice &voice : synth.voices)
    CORE::SHAPER::settle(voice.blocker, BLOCKED, synth.rate);
}
