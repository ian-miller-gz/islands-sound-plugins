// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CLOSED = 0;
constexpr Whole SINGLE = 1;

constexpr Whole WAVES[] = {
  CORE::OSCILLATOR::SAW, CORE::OSCILLATOR::PULSE, CORE::OSCILLATOR::TRIANGLE};
static_assert(std::size(WAVES) == std::size(SUBTRACTIVE::SHAPES));

constexpr Whole KINDS[] = {
  CORE::FILTER::LOW, CORE::FILTER::BAND, CORE::FILTER::HIGH,
  CORE::FILTER::NOTCH};
static_assert(std::size(KINDS) == std::size(SUBTRACTIVE::MODES));

constexpr Whole VOICINGS[] = {CORE::VOICE::POLY, CORE::VOICE::MONO};
constexpr Whole COUNTS[] = {SUBTRACTIVE::VOICES, SINGLE};
static_assert(std::size(VOICINGS) == std::size(SUBTRACTIVE::ASSIGNS));
static_assert(std::size(COUNTS) == std::size(SUBTRACTIVE::ASSIGNS));

void shape(SUBTRACTIVE::Synth &synth) {
  const Float *rows = synth.rows;
  CORE::ENVELOPE::Envelope *envelopes = synth.envelopes;
  const Float close = rows[SUBTRACTIVE::CLOSE];
  envelopes[SUBTRACTIVE::CONTOUR] = CORE::ENVELOPE::ADSR::create(
    rows[SUBTRACTIVE::SWEEP], close, CLOSED, close);
  envelopes[SUBTRACTIVE::LOUDNESS] = CORE::ENVELOPE::ADSR::create(
    rows[SUBTRACTIVE::ATTACK], rows[SUBTRACTIVE::DECAY],
    rows[SUBTRACTIVE::SUSTAIN], rows[SUBTRACTIVE::RELEASE]);
  envelopes[SUBTRACTIVE::LOUDNESS].depth = CORE::ENVELOPE::FULL;
  for (Whole curve = 0; curve < SUBTRACTIVE::CURVES; ++curve)
    CORE::ENVELOPE::shape(envelopes[curve], synth.rate);
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
  synth.shape = ::WAVES[Whole(rows[SHAPE])];
  synth.detune = CORE::PHASE::ratio(rows[SPREAD]);
  synth.kind = ::KINDS[Whole(rows[MODE])];
  ::shape(synth);
  ::allocate(synth);
}
