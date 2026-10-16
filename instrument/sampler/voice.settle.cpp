// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = CORE::PHASE::CENTS / CORE::PHASE::OCTAVE;

struct Voicing {
  Whole mode;
  Flag legato;
  Whole slide;
};

constexpr Voicing VOICINGS[] = {
  {CORE::VOICE::POLY, false, CORE::VOICE::ALWAYS},
  {CORE::VOICE::MONO, false, CORE::VOICE::ALWAYS},
  {CORE::VOICE::MONO, true, CORE::VOICE::LEGATO}};
static_assert(std::size(VOICINGS) == std::size(SAMPLER::MODES));

void allot(SAMPLER::Synth &synth) {
  const Float *rows = synth.rows;
  const Voicing &voicing = VOICINGS[Whole(rows[SAMPLER::MODE])];
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.count = Whole(rows[SAMPLER::VOICES]);
  allocator.mode = voicing.mode;
  allocator.legato = voicing.legato;
  allocator.glide.slide = voicing.slide;
  allocator.glide.time = rows[SAMPLER::GLIDE];
  CORE::VOICE::settle(allocator, synth.pace);
}

void shape(SAMPLER::Synth &synth) {
  const Float *rows = synth.rows;
  CORE::ENVELOPE::Envelope &loudness = synth.loudness;
  loudness = CORE::ENVELOPE::ADSR::create(
    rows[SAMPLER::ATTACK], rows[SAMPLER::DECAY], rows[SAMPLER::SUSTAIN],
    rows[SAMPLER::RELEASE]);
  loudness.curve = Whole(rows[SAMPLER::CURVE]);
  CORE::ENVELOPE::shape(loudness, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::settle(Synth &synth) {
  ::allot(synth);
  ::shape(synth);
}

void SOUND::PLUGINS::SAMPLER::steer(Synth &synth, Whole at) {
  Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  const Float *rows = synth.rows;
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float semis =
    rows[TRACK] * (key - rows[ROOT]) + rows[COARSE] + rows[TUNE] / ::CENT;
  voice.step =
    synth.clips[voice.clip].ratio * CORE::PHASE::ratio(::CENT * semis);
  voice.level =
    CORE::ENVELOPE::weigh(note.velocity, rows[VELOCITY]) * rows[GAIN];
  note.level = voice.loudness.level;
}
