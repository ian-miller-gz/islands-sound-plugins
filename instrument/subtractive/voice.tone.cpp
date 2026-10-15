// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MEAN = 1.0f / Float(SUBTRACTIVE::OSCILLATORS);
constexpr Float CEILING = 0.45f;

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

auto play(SUBTRACTIVE::Synth &synth, Whole at) -> Float {
  SUBTRACTIVE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  for (Whole curve = 0; curve < SUBTRACTIVE::CURVES; ++curve)
    SUBTRACTIVE::breathe(synth.envelopes[curve], voice.gates[curve]);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float mix = ::sing(synth, voice, key);
  const Float lift =
    voice.gates[SUBTRACTIVE::CONTOUR].level * synth.rows[SUBTRACTIVE::DEPTH];
  Float feed = (synth.rows[SUBTRACTIVE::CUTOFF] + lift) * synth.turn;
  if (feed > CEILING) feed = CEILING;
  voice.band += feed * (mix - voice.low - synth.damping * voice.band);
  voice.low += feed * voice.band;
  const SUBTRACTIVE::Gate &loud = voice.gates[SUBTRACTIVE::LOUDNESS];
  if (loud.stage == SUBTRACTIVE::IDLE) note.sounding = false;
  return voice.low * loud.level * note.velocity;
}

}  // namespace

auto SOUND::PLUGINS::SUBTRACTIVE::sound(Synth &synth) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at);
  return sum * synth.rows[GAIN];
}
