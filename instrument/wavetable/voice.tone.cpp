// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
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

auto table(const WAVETABLE::Synth &synth, Whole at)
  -> const CORE::OSCILLATOR::Table & {
  const Whole layout = Whole(synth.rows[::SOURCES[at].table]);
  return synth.tables[layout < WAVETABLE::TABLES ? layout : 0];
}

void tune(WAVETABLE::Synth &synth, WAVETABLE::Voice &voice, Float pitch) {
  const Float *rows = synth.rows;
  const Float second =
    pitch + rows[WAVETABLE::INTERVAL] + rows[WAVETABLE::DETUNE] / CENT;
  const Float pitches[] = {pitch, second};
  for (Whole at = 0; at < WAVETABLE::SOURCES; ++at) {
    CORE::OSCILLATOR::Oscillator &oscillator = voice.oscillators[at];
    CORE::OSCILLATOR::settle(
      oscillator, CORE::PHASE::hertz(pitches[at]), CORE::OSCILLATOR::SQUARE,
      synth.rate);
    const Float step = CORE::PHASE::fraction(oscillator.step) * synth.slope;
    voice.levels[at] = CORE::OSCILLATOR::level(::table(synth, at), step);
  }
}

auto take(
  WAVETABLE::Synth &synth, WAVETABLE::Voice &voice, Whole at,
  Float shift) -> Float {
  CORE::OSCILLATOR::Oscillator &oscillator = voice.oscillators[at];
  const Float position = synth.rows[::SOURCES[at].position] + shift;
  const Float value = CORE::OSCILLATOR::morph(
    ::table(synth, at), position, voice.levels[at],
    WAVETABLE::warp(synth, oscillator.phase));
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
  if (CORE::BLOCK::due(voice.stride, WAVETABLE::STRIDE)) {
    const Float pitch = key + note.cents / CENT + synth.tune +
                        rows[WAVETABLE::BEND] + rows[WAVETABLE::VIBRATO] * lfo;
    ::tune(synth, voice, pitch);
    WAVETABLE::settle(synth, voice, note, key, contour);
  }
  const Float shift = rows[WAVETABLE::SWEEP] * contour +
                      rows[WAVETABLE::SCAN] * lfo +
                      rows[WAVETABLE::PUSH] * note.velocity;
  const Float mix = rows[WAVETABLE::MIX];
  const Float first = ::take(synth, voice, 0, shift);
  const Float second = ::take(synth, voice, 1, shift);
  const Float mixed = LEVEL * ((FULL - mix) * first + mix * second);
  const Float out = WAVETABLE::filter(synth, voice, mixed) * loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return out;
}

}  // namespace

auto SOUND::PLUGINS::WAVETABLE::sound(Synth &synth) -> Sum {
  const Float lfo = CORE::MODULATOR::tick(synth.lfo);
  Sum sum;
  for (Whole at = 0; at < VOICES; ++at) {
    const Float out = synth.weight * ::play(synth, at, lfo);
    CORE::STEREO::add(sum.sides, out, synth.voices[at].sides);
    sum.whole += out;
  }
  return sum;
}
