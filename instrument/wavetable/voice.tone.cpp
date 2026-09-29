// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float DRIVE = 1;
constexpr Float LEVEL = 0.3f;
constexpr Float FULL = 1;

struct Source {
  Whole table;
  Whole position;
};

constexpr Source SOURCES[] = {
  {WAVETABLE::TABLE1, WAVETABLE::POSITION1},
  {WAVETABLE::TABLE2, WAVETABLE::POSITION2}};
static_assert(std::size(SOURCES) == WAVETABLE::SOURCES);

void tune(
  WAVETABLE::Synth &synth, WAVETABLE::Voice &voice, Float pitch, Float key,
  Float contour) {
  const Float *rows = synth.rows;
  const Float second =
    pitch + rows[WAVETABLE::INTERVAL] + rows[WAVETABLE::DETUNE] / CENT;
  const Float pitches[] = {pitch, second};
  for (Whole at = 0; at < WAVETABLE::SOURCES; ++at)
    CORE::OSCILLATOR::settle(
      voice.oscillators[at], CORE::PHASE::hertz(pitches[at]),
      CORE::OSCILLATOR::SQUARE, synth.rate);
  Float octaves =
    rows[WAVETABLE::KEYBOARD] * (key - WAVETABLE::REFERENCE) / OCTAVE;
  octaves += rows[WAVETABLE::ENVELOPE] * contour;
  CORE::FILTER::settle(
    voice.ladder, rows[WAVETABLE::CUTOFF] * std::exp2(octaves),
    rows[WAVETABLE::RESONANCE], DRIVE, synth.rate);
  voice.stale = false;
}

auto take(
  WAVETABLE::Synth &synth, WAVETABLE::Voice &voice, Whole at,
  Float shift) -> Float {
  const Source &source = ::SOURCES[at];
  const Whole layout = Whole(synth.rows[source.table]);
  const CORE::OSCILLATOR::Table &table =
    synth.tables[layout < WAVETABLE::TABLES ? layout : 0];
  CORE::OSCILLATOR::Oscillator &oscillator = voice.oscillators[at];
  const Float position = synth.rows[source.position] + shift;
  const Float value =
    CORE::OSCILLATOR::morph(table, position, oscillator.phase);
  oscillator.phase += oscillator.step;
  return value;
}

auto play(WAVETABLE::Synth &synth, Whole at, Float lfo) -> Float {
  WAVETABLE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.door, synth.door);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float pitch = key + note.cents / CENT + synth.tune +
                      rows[WAVETABLE::BEND] + rows[WAVETABLE::VIBRATO] * lfo;
  if (voice.stale || synth.clock == 0)
    ::tune(synth, voice, pitch, key, contour);
  const Float shift =
    rows[WAVETABLE::SWEEP] * contour + rows[WAVETABLE::SCAN] * lfo;
  const Float mix = rows[WAVETABLE::MIX];
  const Float first = ::take(synth, voice, 0, shift);
  const Float second = ::take(synth, voice, 1, shift);
  const Float mixed = LEVEL * ((FULL - mix) * first + mix * second);
  const Float out = CORE::FILTER::tick(voice.ladder, mixed) * loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return out;
}

}  // namespace

auto SOUND::WAVETABLE::sound(Synth &synth) -> Float {
  const Float lfo = CORE::MODULATOR::tick(synth.lfo);
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at, lfo);
  synth.clock = (synth.clock + 1) % CONTROL;
  return sum;
}
