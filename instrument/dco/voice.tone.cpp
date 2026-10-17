// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float EIGHT = 1;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float DETUNE = 8;

struct Sources {
  Float lfo = 0;
  Float noise = 0;
};

auto pitch(const DCO::Synth &synth, Whole at, Float key, Float lfo) -> Float {
  const Float *rows = synth.rows;
  const Float range = (rows[DCO::RANGE] - EIGHT) * OCTAVE;
  const Float trim = DETUNE * rows[DCO::TRIM] * synth.trims[at].pitch;
  const Float wander = rows[DCO::DRIFT] * synth.voices[at].drift.value;
  const Float cents = synth.allocator.notes[at].cents + trim + wander;
  return key + range + cents / CENT + synth.tune + rows[DCO::BEND] +
         rows[DCO::VIBRATO] * lfo;
}

auto shape(const Float *rows, Float contour, Float lfo) -> Float {
  const Float shapes[] = {(lfo + FULL) * HALF, FULL, contour};
  const Whole at = Whole(rows[DCO::SOURCE]);
  return at < std::size(shapes) ? shapes[at] : FULL;
}

auto play(DCO::Synth &synth, Whole at, Sources sources) -> Float {
  DCO::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour =
    CORE::ENVELOPE::tick(voice.filter.gate, voice.filter.shape);
  const Float loud =
    CORE::ENVELOPE::tick(voice.amplifier.gate, voice.amplifier.shape);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  CORE::MODULATOR::tick(voice.drift);
  const Float played = ::pitch(synth, at, key, sources.lfo);
  const Float width = ::shape(rows, contour, sources.lfo);
  const Float mixed =
    DCO::sing(synth, voice, played, width) + rows[DCO::NOISE] * sources.noise;
  const Float out = DCO::filter(synth, at, mixed, contour, sources.lfo) * loud;
  if (!CORE::ENVELOPE::sounding(voice.amplifier.gate)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

}  // namespace

auto SOUND::PLUGINS::DCO::sound(Synth &synth) -> Pair {
  const Sources sources = {
    CORE::MODULATOR::tick(synth.lfo), CORE::NOISE::tick(synth.noise)};
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at, sources);
  const Float level = synth.level * sum;
  const Float passed = CORE::FILTER::tick(synth.highpass, level);
  const CORE::STEREO::Pair pair = CORE::LINE::tick(synth.chorus, passed);
  return {pair.left, pair.right, synth.chorus.whole};
}
