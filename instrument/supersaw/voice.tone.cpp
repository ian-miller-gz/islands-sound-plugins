// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float LEVEL = 0.3f;
constexpr Float FULL = 1;

void tune(
  SUPERSAW::Synth &synth, SUPERSAW::Voice &voice, const CORE::VOICE::Note &note,
  Float key, Float contour, Float lfo) {
  const Float *rows = synth.rows;
  const Float pitch = key + note.cents / CENT + synth.tune +
                      rows[SUPERSAW::BEND] + rows[SUPERSAW::VIBRATO] * lfo;
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float detune = rows[SUPERSAW::DETUNE];
  CORE::OSCILLATOR::settle(
    voice.super, hertz, detune, rows[SUPERSAW::MIX], synth.rate);
  CORE::FILTER::settle(
    voice.highpass, CORE::FILTER::HIGH, hertz * detune, CORE::FILTER::FLAT, 0,
    synth.rate);
  Float octaves =
    rows[SUPERSAW::KEYBOARD] * (key - SUPERSAW::REFERENCE) / OCTAVE;
  octaves += rows[SUPERSAW::ENVELOPE] * contour + rows[SUPERSAW::WOBBLE] * lfo;
  octaves += rows[SUPERSAW::TOUCH] * note.velocity;
  SUPERSAW::settle(synth, voice, rows[SUPERSAW::CUTOFF] * std::exp2(octaves));
}

auto play(SUPERSAW::Synth &synth, Whole at, Float lfo) -> Float {
  SUPERSAW::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.door, synth.door);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  if (CORE::BLOCK::due(voice.stride, SUPERSAW::STRIDE))
    ::tune(synth, voice, note, key, contour, lfo);
  const Float saws = LEVEL * CORE::OSCILLATOR::tick(voice.super);
  const Float thinned = CORE::FILTER::tick(voice.highpass, saws);
  const Float out = SUPERSAW::filter(synth, voice, thinned) * loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return out;
}

auto lean(Float pan) -> Float { return pan > 0 ? FULL - pan : FULL; }

}  // namespace

auto SOUND::PLUGINS::SUPERSAW::sound(Synth &synth) -> Pair {
  Pair pair;
  const Float lfo = CORE::MODULATOR::tick(synth.lfo);
  for (Whole at = 0; at < VOICES; ++at) {
    const Float out = ::play(synth, at, lfo);
    const Float pan = synth.allocator.notes[at].pan;
    pair.left += out * ::lean(pan);
    pair.right += out * ::lean(-pan);
    pair.whole += out;
  }
  const CORE::STEREO::Pair wet = CORE::LINE::tick(synth.chorus, pair.whole);
  pair.left += wet.left - pair.whole;
  pair.right += wet.right - pair.whole;
  pair.whole = synth.chorus.whole;
  return pair;
}
