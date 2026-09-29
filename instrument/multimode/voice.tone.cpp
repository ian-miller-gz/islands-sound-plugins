// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float PEAK = 0.9f;
constexpr Float SPAN = MULTIMODE::SQUARE - MULTIMODE::NARROW;
constexpr Float PANS[MULTIMODE::VOICES] = {-FULL, FULL};

auto pitch(
  const MULTIMODE::Synth &synth, const CORE::VOICE::Note &note, Float key,
  Float source) -> Float {
  const Float *rows = synth.rows;
  return key + note.cents / CENT + synth.tune + rows[MULTIMODE::BEND] +
         rows[MULTIMODE::MODULATION] * source;
}

auto swept(const MULTIMODE::Synth &synth, Float key, Float contour, Float lfo)
  -> Float {
  const Float *rows = synth.rows;
  Float octaves =
    rows[MULTIMODE::TRACKING] * (key - MULTIMODE::REFERENCE) / OCTAVE;
  octaves +=
    rows[MULTIMODE::ENVELOPE] * contour + rows[MULTIMODE::WOBBLE] * lfo;
  return rows[MULTIMODE::CUTOFF] * std::exp2(octaves);
}

auto play(MULTIMODE::Synth &synth, Whole at, Float lfo) -> Float {
  MULTIMODE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.loudness, synth.loudness);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float source = rows[MULTIMODE::MODULATOR] > 0 ? contour : lfo;
  const Float pwm = rows[MULTIMODE::SOURCE] > 0 ? contour : (lfo + FULL) * HALF;
  const Float width =
    rows[MULTIMODE::WIDTH] - SPAN * rows[MULTIMODE::SWEEP] * pwm;
  const Float mixed =
    MULTIMODE::sing(synth, voice, ::pitch(synth, note, key, source), width);
  const Float cutoff = ::swept(synth, key, contour, lfo);
  const Float out = MULTIMODE::sweep(synth, voice, mixed, cutoff) * loud;
  if (!CORE::ENVELOPE::sounding(voice.loudness)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

auto gain(Float pan) -> Float { return pan > 0 ? FULL - pan : FULL; }

}  // namespace

auto SOUND::PLUGINS::MULTIMODE::sweep(
  Synth &synth, Voice &voice, Float in, Float cutoff) -> Float {
  const Float *rows = synth.rows;
  CORE::FILTER::settle(
    voice.variable, CORE::FILTER::LOW, cutoff, rows[RESONANCE] * PEAK,
    synth.rate);
  const CORE::FILTER::Taps taps = CORE::FILTER::split(voice.variable, in);
  if (rows[BAND] > 0) return taps.band;
  return taps.low + (taps.high - taps.low) * rows[MODE];
}

auto SOUND::PLUGINS::MULTIMODE::sound(Synth &synth) -> Pair {
  const Float lfo = CORE::MODULATOR::tick(synth.lfo);
  const Float spread = synth.rows[SPREAD];
  Pair pair;
  for (Whole at = 0; at < VOICES; ++at) {
    const Float out = ::play(synth, at, lfo);
    const Float pan = PANS[at] * spread;
    pair.left += out * ::gain(pan);
    pair.right += out * ::gain(-pan);
    pair.whole += out;
  }
  return pair;
}
