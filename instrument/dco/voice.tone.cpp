// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float EIGHT = 1;
constexpr Float DRIVE = 1;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;

struct Sources {
  Float lfo = 0;
  Float noise = 0;
};

auto pitch(
  const DCO::Synth &synth, const CORE::VOICE::Note &note, Float key,
  Float lfo) -> Float {
  const Float *rows = synth.rows;
  const Float range = (rows[DCO::RANGE] - EIGHT) * OCTAVE;
  return key + range + note.cents / CENT + synth.tune + rows[DCO::BEND] +
         rows[DCO::VIBRATO] * lfo;
}

auto shape(const Float *rows, Float contour, Float lfo) -> Float {
  const Float shapes[] = {(lfo + FULL) * HALF, FULL, contour};
  const Whole at = Whole(rows[DCO::SOURCE]);
  return at < std::size(shapes) ? shapes[at] : FULL;
}

auto swept(const DCO::Synth &synth, Float key, Float contour, Float lfo)
  -> Float {
  const Float *rows = synth.rows;
  const Float sign = rows[DCO::POLARITY] > 0 ? -FULL : FULL;
  Float octaves = rows[DCO::KEYBOARD] * (key - DCO::REFERENCE) / OCTAVE;
  octaves += sign * rows[DCO::ENVELOPE] * contour + rows[DCO::WOBBLE] * lfo;
  return rows[DCO::CUTOFF] * std::exp2(octaves);
}

auto play(DCO::Synth &synth, Whole at, Sources sources) -> Float {
  DCO::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.door, synth.door);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float played = ::pitch(synth, note, key, sources.lfo);
  const Float width = ::shape(rows, contour, sources.lfo);
  const Float mixed =
    DCO::sing(synth, voice, played, width) + rows[DCO::NOISE] * sources.noise;
  const Float cutoff = ::swept(synth, key, contour, sources.lfo);
  CORE::FILTER::settle(
    voice.ladder, cutoff, rows[DCO::RESONANCE], DRIVE, synth.rate);
  const Float out = CORE::FILTER::tick(voice.ladder, mixed) * loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

}  // namespace

auto SOUND::PLUGINS::DCO::sound(Synth &synth) -> Pair {
  const Sources sources = {
    CORE::MODULATOR::tick(synth.lfo), CORE::NOISE::tick(synth.noise)};
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at, sources);
  return tick(synth.chorus, CORE::FILTER::tick(synth.highpass, sum));
}
