// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LOOSE = 2.0f;
constexpr Float SQUEEZE = 2.0f;
constexpr Float CLOSED = 0;
constexpr Whole SINGLE = 1;

constexpr Whole VOICINGS[] = {CORE::VOICE::POLY, CORE::VOICE::MONO};
constexpr Whole COUNTS[] = {SUBTRACTIVE::VOICES, SINGLE};
static_assert(std::size(VOICINGS) == std::size(SUBTRACTIVE::ASSIGNS));
static_assert(std::size(COUNTS) == std::size(SUBTRACTIVE::ASSIGNS));

void contour(SUBTRACTIVE::Synth &synth) {
  const Float *rows = synth.rows;
  SUBTRACTIVE::Envelope &contour = synth.envelopes[SUBTRACTIVE::CONTOUR];
  contour.attack = rows[SUBTRACTIVE::SWEEP];
  contour.decay = rows[SUBTRACTIVE::CLOSE];
  contour.sustain = CLOSED;
  contour.release = rows[SUBTRACTIVE::CLOSE];
}

void loudness(SUBTRACTIVE::Synth &synth) {
  const Float *rows = synth.rows;
  SUBTRACTIVE::Envelope &loudness = synth.envelopes[SUBTRACTIVE::LOUDNESS];
  loudness.attack = rows[SUBTRACTIVE::ATTACK];
  loudness.decay = rows[SUBTRACTIVE::DECAY];
  loudness.sustain = rows[SUBTRACTIVE::SUSTAIN];
  loudness.release = rows[SUBTRACTIVE::RELEASE];
}

void allocate(SUBTRACTIVE::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.mode = SUBTRACTIVE::mode(synth);
  allocator.count = COUNTS[Whole(synth.rows[SUBTRACTIVE::ASSIGN])];
  allocator.glide.time = synth.rows[SUBTRACTIVE::GLIDE];
  CORE::VOICE::settle(allocator, synth.rate);
}

}  // namespace

auto SOUND::PLUGINS::SUBTRACTIVE::mode(const Synth &synth) -> Whole {
  return ::VOICINGS[Whole(synth.rows[ASSIGN])];
}

void SOUND::PLUGINS::SUBTRACTIVE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.shape = Whole(rows[SHAPE]);
  synth.detune = CORE::PHASE::ratio(rows[SPREAD]);
  synth.damping = ::LOOSE - rows[RESONANCE] * ::SQUEEZE;
  ::contour(synth);
  ::loudness(synth);
  for (Envelope &envelope : synth.envelopes) shape(envelope, synth.rate);
  ::allocate(synth);
}
