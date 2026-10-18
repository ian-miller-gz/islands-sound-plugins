// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MEAN = 1.0f / Float(SUBTRACTIVE::OSCILLATORS);

auto sing(SUBTRACTIVE::Synth &synth, SUBTRACTIVE::Voice &voice, Float key)
  -> Float {
  const Float hertz = CORE::PHASE::hertz(key);
  const Float frequencies[] = {hertz, hertz * synth.detune};
  static_assert(std::size(frequencies) == SUBTRACTIVE::OSCILLATORS);
  Float mix = 0;
  for (Whole at = 0; at < SUBTRACTIVE::OSCILLATORS; ++at) {
    CORE::OSCILLATOR::Oscillator &oscillator = voice.oscillators[at];
    CORE::OSCILLATOR::settle(
      oscillator, frequencies[at], CORE::OSCILLATOR::SQUARE, synth.rate);
    mix += CORE::OSCILLATOR::tick(oscillator, synth.shape);
  }
  return mix * MEAN;
}

auto swept(const SUBTRACTIVE::Synth &synth, Float key, Float contour) -> Float {
  const Float *rows = synth.rows;
  const Float octaves = rows[SUBTRACTIVE::KEYBOARD] *
                        (key - SUBTRACTIVE::REFERENCE) / CORE::PHASE::OCTAVE;
  const Float base =
    rows[SUBTRACTIVE::CUTOFF] + rows[SUBTRACTIVE::DEPTH] * contour;
  return base * std::exp2(octaves);
}

auto play(SUBTRACTIVE::Synth &synth, Whole at) -> Float {
  SUBTRACTIVE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  CORE::ENVELOPE::Gate *gates = voice.gates;
  const CORE::ENVELOPE::Envelope *envelopes = synth.envelopes;
  const Float contour = CORE::ENVELOPE::tick(
    gates[SUBTRACTIVE::CONTOUR], envelopes[SUBTRACTIVE::CONTOUR]);
  const Float loud = CORE::ENVELOPE::tick(
    gates[SUBTRACTIVE::LOUDNESS], envelopes[SUBTRACTIVE::LOUDNESS]);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  CORE::FILTER::settle(
    voice.variable, synth.kind, ::swept(synth, key, contour),
    synth.rows[SUBTRACTIVE::RESONANCE], synth.rate);
  const Float out =
    CORE::FILTER::tick(voice.variable, ::sing(synth, voice, key));
  if (!CORE::ENVELOPE::sounding(gates[SUBTRACTIVE::LOUDNESS]))
    note.sounding = false;
  return out * loud;
}

}  // namespace

auto SOUND::PLUGINS::SUBTRACTIVE::sound(Synth &synth) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at);
  return sum * synth.rows[GAIN];
}
