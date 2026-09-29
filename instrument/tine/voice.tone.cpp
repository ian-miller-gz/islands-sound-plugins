// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1;
constexpr Float HALF = 0.5f;
constexpr Float FLOOR = 0.15f;
constexpr Float MIX = 0.5f;
constexpr Float TWICE = 2;

auto play(TINE::Synth &synth, Whole at) -> Float {
  TINE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const auto &sine = synth.sine;
  const Float loud = CORE::ENVELOPE::tick(voice.door, voice.loudness);
  const Float strike = CORE::ENVELOPE::tick(voice.strike, synth.strike);
  const Float ring = CORE::ENVELOPE::tick(voice.ring, synth.ring);
  const Float index = rows[TINE::TONE] * (strike + FLOOR);
  const Float bar = CORE::OSCILLATOR::tick(voice.bar, sine, 0, FULL);
  const Float coupling = rows[TINE::COUPLING];
  const Float tine =
    CORE::OSCILLATOR::tick(voice.tine, sine, index * bar, FULL);
  const Float coupled = tine * (FULL + coupling * bar) / (FULL + coupling);
  const Float chime = rows[TINE::BELL] * voice.chime * ring;
  const Float bell = CORE::OSCILLATOR::tick(voice.bell, sine, 0, chime);
  const Float heard = TINE::pick(synth, (coupled + bell) * loud);
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, heard);
}

}  // namespace

auto SOUND::PLUGINS::TINE::pick(const Synth &synth, Float displacement)
  -> Float {
  const Float bias = synth.rows[VOICING];
  const Float driven = synth.drive * displacement + bias;
  return (CORE::SHAPER::soft(driven) - CORE::SHAPER::soft(bias)) / synth.drive;
}

auto SOUND::PLUGINS::TINE::sound(Synth &synth) -> Pair {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at);
  sum *= MIX;
  const Float *rows = synth.rows;
  const Float swing = CORE::MODULATOR::tick(synth.tremolo);
  const Float depth = HALF * rows[DEPTH];
  const Float mirrored = (FULL - TWICE * rows[STEREO]) * swing;
  const Float left = sum * (FULL - depth * (FULL + swing));
  const Float right = sum * (FULL - depth * (FULL + mirrored));
  return {left, right, HALF * (left + right)};
}
