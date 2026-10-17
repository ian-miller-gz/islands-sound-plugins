// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float DRIVE = 1;
constexpr Float DETUNE = 8;
constexpr Float SHIFT = 0.25f;

struct Sources {
  Float lfo = 0;
  Float noise = 0;
};

auto offset(const POLYMOD::Synth &synth, Whole at, Float lfo) -> Float {
  const Float *rows = synth.rows;
  const Float trim = DETUNE * rows[POLYMOD::TRIM] * synth.trims[at].pitch;
  const Float wander = rows[POLYMOD::DRIFT] * synth.voices[at].drift.value;
  const Float cents = synth.allocator.notes[at].cents + trim + wander;
  return cents / CENT + synth.tune + rows[POLYMOD::BEND] +
         rows[POLYMOD::VIBRATO] * lfo;
}

auto swept(
  const POLYMOD::Synth &synth, Whole at, Float contour, Float lfo,
  Float mod) -> Float {
  const Float *rows = synth.rows;
  const CORE::VOICE::Note &note = synth.allocator.notes[at];
  const Float key = note.current - POLYMOD::REFERENCE;
  Float octaves = rows[POLYMOD::TRACKING] * key / OCTAVE;
  octaves += rows[POLYMOD::AMOUNT] * contour + rows[POLYMOD::WOBBLE] * lfo;
  if (rows[POLYMOD::FILTER] > 0) octaves += POLYMOD::OCTAVES * mod;
  octaves += rows[POLYMOD::TOUCH] * note.velocity;
  octaves += SHIFT * rows[POLYMOD::TRIM] * synth.trims[at].cutoff;
  return rows[POLYMOD::CUTOFF] * std::exp2(octaves);
}

auto filter(
  POLYMOD::Synth &synth, Whole at, Float in, Float contour, Float lfo,
  Float mod) -> Float {
  const Float cutoff = ::swept(synth, at, contour, lfo, mod);
  CORE::FILTER::Ladder &ladder = synth.voices[at].ladder;
  CORE::FILTER::settle(
    ladder, cutoff, synth.rows[POLYMOD::RESONANCE], DRIVE, synth.rate);
  return CORE::FILTER::tick(ladder, in);
}

auto play(POLYMOD::Synth &synth, Whole at, Sources sources) -> Float {
  POLYMOD::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour =
    CORE::ENVELOPE::tick(voice.contour.gate, voice.contour.shape);
  const Float loud =
    CORE::ENVELOPE::tick(voice.loudness.gate, voice.loudness.shape);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  CORE::MODULATOR::tick(voice.drift);
  const Float bent = ::offset(synth, at, sources.lfo);
  const Float played = rows[POLYMOD::KEYBOARD] > 0 ? key : POLYMOD::REFERENCE;
  const Float b = POLYMOD::modulate(synth, voice, played + bent);
  const Float mod =
    rows[POLYMOD::CONTOUR] * contour + rows[POLYMOD::MODULATOR] * b;
  const Float a = POLYMOD::sing(synth, voice, key + bent, mod);
  const Float mixed = rows[POLYMOD::LEVELA] * a + rows[POLYMOD::LEVELB] * b +
                      rows[POLYMOD::NOISE] * sources.noise;
  const Float out =
    ::filter(synth, at, mixed, contour, sources.lfo, mod) * loud;
  if (!CORE::ENVELOPE::sounding(voice.loudness.gate)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

}  // namespace

auto SOUND::PLUGINS::POLYMOD::sound(Synth &synth) -> Pair {
  const Sources sources = {
    CORE::MODULATOR::tick(synth.lfo), CORE::NOISE::tick(synth.noise)};
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at, sources);
  const CORE::STEREO::Pair pair = CORE::LINE::tick(synth.chorus, sum);
  return {pair.left, pair.right, synth.chorus.whole};
}
