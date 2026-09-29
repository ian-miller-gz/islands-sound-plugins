// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float DRIVE = 1;
constexpr Float LEVEL = 0.3f;
constexpr Float FULL = 1;

void tune(
  SUPERSAW::Synth &synth, SUPERSAW::Voice &voice, const CORE::VOICE::Note &note,
  Float key, Float contour) {
  const Float *rows = synth.rows;
  const Float pitch =
    key + note.cents / CENT + synth.tune + rows[SUPERSAW::BEND];
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float detune = rows[SUPERSAW::DETUNE];
  CORE::OSCILLATOR::settle(
    voice.super, hertz, detune, rows[SUPERSAW::MIX], synth.rate);
  CORE::FILTER::settle(
    voice.highpass, CORE::FILTER::HIGH, hertz * detune, CORE::FILTER::FLAT, 0,
    synth.rate);
  Float octaves =
    rows[SUPERSAW::KEYBOARD] * (key - SUPERSAW::REFERENCE) / OCTAVE;
  octaves += rows[SUPERSAW::ENVELOPE] * contour;
  CORE::FILTER::settle(
    voice.ladder, rows[SUPERSAW::CUTOFF] * std::exp2(octaves),
    rows[SUPERSAW::RESONANCE], DRIVE, synth.rate);
  voice.stale = false;
}

auto play(SUPERSAW::Synth &synth, Whole at) -> Float {
  SUPERSAW::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.door, synth.door);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  if (voice.stale || synth.clock == 0) ::tune(synth, voice, note, key, contour);
  const Float saws = LEVEL * CORE::OSCILLATOR::tick(voice.super);
  const Float thinned = CORE::FILTER::tick(voice.highpass, saws);
  const Float out = CORE::FILTER::tick(voice.ladder, thinned) * loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return out;
}

auto lean(Float pan) -> Float { return pan > 0 ? FULL - pan : FULL; }

}  // namespace

auto SOUND::PLUGINS::SUPERSAW::sound(Synth &synth) -> Pair {
  Pair pair;
  for (Whole at = 0; at < VOICES; ++at) {
    const Float out = ::play(synth, at);
    const Float pan = synth.allocator.notes[at].pan;
    pair.left += out * ::lean(pan);
    pair.right += out * ::lean(-pan);
    pair.whole += out;
  }
  synth.clock = (synth.clock + 1) % CONTROL;
  return pair;
}
