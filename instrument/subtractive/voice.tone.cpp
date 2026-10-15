// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SCALE = 1.0f / 2147483648.0f;
constexpr Float PAIR = 0.5f;
constexpr Float CEILING = 0.45f;

auto wave(Whole shape, uint32_t phase) -> Float {
  const Float ramp = Float(static_cast<int32_t>(phase)) * SCALE;
  switch (shape) {
    case SUBTRACTIVE::PULSE:
      return ramp < 0 ? -1.0f : 1.0f;
    case SUBTRACTIVE::TRIANGLE:
      return (ramp < 0 ? -ramp : ramp) * 2.0f - 1.0f;
    default:
      return ramp;
  }
}

auto sing(SUBTRACTIVE::Synth &synth, SUBTRACTIVE::Voice &voice, Float key)
  -> Float {
  const CORE::PHASE::Wheel step =
    CORE::PHASE::step(CORE::PHASE::hertz(key), synth.rate);
  const CORE::PHASE::Wheel steps[] = {
    step, CORE::PHASE::Wheel(Float(step) * synth.detune)};
  Float mix = 0;
  for (Whole at = 0; at < SUBTRACTIVE::OSCILLATORS; ++at) {
    mix += ::wave(synth.shape, voice.phases[at]);
    voice.phases[at] += steps[at];
  }
  return mix * PAIR;
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
