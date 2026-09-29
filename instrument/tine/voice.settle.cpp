// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float ONSET = 0.001f;
constexpr Float MIDDLE = 60;
constexpr Float OCTAVE = 12;
constexpr Float TRACK = 0.5f;
constexpr Float CENT = 100;
constexpr Float BAR = 1;
constexpr Float OVERTONE = 6.267f;
constexpr Float CEILING = 0.45f;
constexpr Float DRIVE = 4;
constexpr Float BLOCKED = 10;

auto fading(Float seconds, Float depth, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  auto envelope = CORE::ENVELOPE::AD::create(0, seconds);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.depth = depth;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void swing(TINE::Synth &synth) {
  synth.tremolo.wave = CORE::MODULATOR::SINE;
  synth.tremolo.hertz = synth.rows[TINE::RATE];
  CORE::MODULATOR::settle(synth.tremolo, synth.rate);
}

}  // namespace

void SOUND::TINE::shape(Synth &synth, Whole at) {
  const Float *rows = synth.rows;
  const Float key = Float(synth.allocator.notes[at].pitch);
  const Float decay = rows[DECAY] * std::exp2(TRACK * (MIDDLE - key) / OCTAVE);
  auto &envelope = synth.voices[at].loudness;
  envelope = CORE::ENVELOPE::ADSR::create(ONSET, decay, 0, rows[RELEASE]);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  envelope.depth = rows[VELOCITY];
  CORE::ENVELOPE::shape(envelope, synth.rate);
}

void SOUND::TINE::tune(Synth &synth, Whole at) {
  const Float *rows = synth.rows;
  const Float key = Float(synth.allocator.notes[at].pitch);
  const Float hertz = CORE::PHASE::hertz(key + rows[TUNE] / CENT + rows[BEND]);
  const Float bell = hertz * OVERTONE;
  Voice &voice = synth.voices[at];
  CORE::OSCILLATOR::settle(voice.tine, hertz, 0, synth.rate);
  CORE::OSCILLATOR::settle(voice.bar, hertz * BAR, 0, synth.rate);
  CORE::OSCILLATOR::settle(voice.bell, bell, 0, synth.rate);
  voice.chime = bell < CEILING * Float(synth.rate) ? 1 : 0;
}

void SOUND::TINE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.strike = ::fading(rows[STRIKE], rows[VELOCITY], synth.rate);
  synth.ring = ::fading(rows[RING], rows[VELOCITY], synth.rate);
  synth.drive = 1 + DRIVE * rows[PICKUP];
  ::swing(synth);
  CORE::VOICE::settle(synth.allocator, synth.rate);
  for (Whole at = 0; at < VOICES; ++at) {
    CORE::SHAPER::settle(synth.voices[at].blocker, BLOCKED, synth.rate);
    if (!synth.allocator.notes[at].sounding) continue;
    shape(synth, at);
    tune(synth, at);
  }
}
