// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float DRIVE = 1;

struct Sources {
  Float lfo = 0;
  Float noise = 0;
};

auto offset(
  const POLYMOD::Synth &synth, const CORE::VOICE::Note &note,
  Float lfo) -> Float {
  const Float *rows = synth.rows;
  return note.cents / CENT + synth.tune + rows[POLYMOD::BEND] +
         rows[POLYMOD::VIBRATO] * lfo;
}

auto swept(
  const POLYMOD::Synth &synth, Float key, Float contour, Float lfo,
  Float mod) -> Float {
  const Float *rows = synth.rows;
  Float octaves = rows[POLYMOD::TRACKING] * (key - POLYMOD::REFERENCE) / OCTAVE;
  octaves += rows[POLYMOD::AMOUNT] * contour + rows[POLYMOD::WOBBLE] * lfo;
  if (rows[POLYMOD::FILTER] > 0) octaves += POLYMOD::OCTAVES * mod;
  return rows[POLYMOD::CUTOFF] * std::exp2(octaves);
}

auto play(POLYMOD::Synth &synth, Whole at, Sources sources) -> Float {
  POLYMOD::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.loudness, synth.loudness);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float bent = ::offset(synth, note, sources.lfo);
  const Float played = rows[POLYMOD::KEYBOARD] > 0 ? key : POLYMOD::REFERENCE;
  const Float b = POLYMOD::modulate(synth, voice, played + bent);
  const Float mod =
    rows[POLYMOD::CONTOUR] * contour + rows[POLYMOD::MODULATOR] * b;
  const Float a = POLYMOD::sing(synth, voice, key + bent, mod);
  const Float mixed = rows[POLYMOD::LEVELA] * a + rows[POLYMOD::LEVELB] * b +
                      rows[POLYMOD::NOISE] * sources.noise;
  const Float cutoff = ::swept(synth, key, contour, sources.lfo, mod);
  CORE::FILTER::settle(
    voice.ladder, cutoff, rows[POLYMOD::RESONANCE], DRIVE, synth.rate);
  const Float out = CORE::FILTER::tick(voice.ladder, mixed) * loud;
  if (!CORE::ENVELOPE::sounding(voice.loudness)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

}  // namespace

auto SOUND::PLUGINS::POLYMOD::sound(Synth &synth) -> Float {
  const Sources sources = {
    CORE::MODULATOR::tick(synth.lfo), CORE::NOISE::tick(synth.noise)};
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at, sources);
  return sum;
}
