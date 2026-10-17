// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1;
constexpr Float HALF = 0.5f;
constexpr Float FLOOR = 0.15f;
constexpr Float MIX = 0.5f;
constexpr Float TWICE = 2;
constexpr Float QUIET = 0.0001f;

auto tone(TINE::Synth &synth, TINE::Voice &voice) -> Float {
  const Float *rows = synth.rows;
  const auto &sine = synth.sine;
  const Float loud = CORE::ENVELOPE::tick(voice.door, voice.loudness);
  const Float strike = CORE::ENVELOPE::tick(voice.strike, synth.strike);
  const Float ring = CORE::ENVELOPE::tick(voice.ring, synth.ring);
  const Float index = rows[TINE::TONE] * (strike + FLOOR) * voice.taper;
  const Float bar = CORE::OSCILLATOR::tick(voice.bar, sine, 0, FULL);
  const Float coupling = rows[TINE::COUPLING];
  const Float tine =
    CORE::OSCILLATOR::tick(voice.tine, sine, index * bar, FULL);
  const Float coupled = tine * (FULL + coupling * bar) / (FULL + coupling);
  const Float chime = rows[TINE::BELL] * voice.chime * ring * voice.taper;
  const Float bell = CORE::OSCILLATOR::tick(voice.bell, sine, 0, chime);
  return TINE::pick(synth, (coupled + bell) * loud);
}

auto play(TINE::Synth &synth, Whole at) -> Float {
  TINE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float heard = CORE::FILTER::tick(voice.low, ::tone(synth, voice));
  const Float thump = CORE::NOISE::tick(voice.hammer);
  const Float stop = CORE::NOISE::tick(voice.damper);
  const Flag quiet = voice.damper.level < QUIET;
  if (!CORE::ENVELOPE::sounding(voice.door) && quiet) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, heard + thump + stop);
}

}  // namespace

auto SOUND::PLUGINS::TINE::pick(const Synth &synth, Float displacement)
  -> Float {
  const Float bias = synth.rows[VOICING];
  const Float driven = synth.drive * displacement + bias;
  return (CORE::SHAPER::soft(driven) - CORE::SHAPER::soft(bias)) / synth.drive;
}

auto SOUND::PLUGINS::TINE::sound(Synth &synth) -> CORE::STEREO::Pair {
  CORE::STEREO::Pair sum;
  for (Whole at = 0; at < VOICES; ++at)
    CORE::STEREO::add(sum, MIX * ::play(synth, at), synth.voices[at].sides);
  const Float *rows = synth.rows;
  const Float swing = CORE::MODULATOR::tick(synth.tremolo);
  const Float depth = HALF * rows[DEPTH];
  const Float mirrored = (FULL - TWICE * rows[STEREO]) * swing;
  const Float left = sum.left * (FULL - depth * (FULL + swing));
  const Float right = sum.right * (FULL - depth * (FULL + mirrored));
  return {left, right};
}
