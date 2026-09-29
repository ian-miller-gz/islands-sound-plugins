// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float OCTAVE = 12;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float SPAN = 0.45f;
constexpr Float DRIVE = 1;

auto swing(MONO::Synth &synth, Float noise) -> Float {
  const CORE::PHASE::Wheel before = synth.lfo.phase;
  const Float value = CORE::MODULATOR::tick(synth.lfo);
  const Flag cycled = Whole(synth.rows[MONO::TRIGGER]) == MONO::CYCLED;
  const Flag held = synth.allocator.notes[0].held;
  if (cycled && held && synth.lfo.phase < before)
    MONO::strike(synth.envelope, synth.allocator.notes[0].velocity);
  return Whole(synth.rows[MONO::WAVE]) == MONO::NOISY ? noise : value;
}

auto width(const MONO::Synth &synth, Float lfo, Float contour) -> Float {
  const Float sources[] = {(lfo + FULL) * HALF, FULL, contour};
  const Float depth = sources[Whole(synth.rows[MONO::SOURCE])];
  return CORE::OSCILLATOR::SQUARE - SPAN * synth.rows[MONO::WIDTH] * depth;
}

auto mixed(MONO::Synth &synth, Float pitch, Float width) -> Float {
  CORE::OSCILLATOR::Oscillator &main = synth.oscillator;
  CORE::OSCILLATOR::Oscillator &sub = synth.sub;
  const Float hertz = CORE::PHASE::hertz(pitch);
  CORE::OSCILLATOR::settle(main, hertz, width, synth.rate);
  CORE::OSCILLATOR::settle(
    sub, hertz / synth.divisor, synth.narrow, synth.rate);
  Float sum = synth.rows[MONO::PULSE] * CORE::OSCILLATOR::pulse(main);
  sum += synth.rows[MONO::SAW] * CORE::OSCILLATOR::saw(main);
  sum += synth.rows[MONO::SUB] * CORE::OSCILLATOR::pulse(sub);
  main.phase += main.step;
  sub.phase += sub.step;
  return sum;
}

auto swept(const MONO::Synth &synth, Float key, Float lfo, Float contour)
  -> Float {
  Float octaves = synth.rows[MONO::KEYBOARD] * (key - MONO::REFERENCE) / OCTAVE;
  octaves += synth.rows[MONO::ENVELOPE] * contour;
  octaves += synth.rows[MONO::MODULATION] * lfo;
  return synth.rows[MONO::CUTOFF] * std::exp2(octaves);
}

auto quiet(const MONO::Synth &synth) -> Flag {
  return !CORE::ENVELOPE::sounding(synth.envelope.gate) &&
         !CORE::ENVELOPE::sounding(synth.gate.gate);
}

}  // namespace

auto SOUND::MONO::sound(Synth &synth) -> Float {
  CORE::VOICE::Note &note = synth.allocator.notes[0];
  const Float noise = CORE::NOISE::tick(synth.noise);
  const Float lfo = ::swing(synth, noise);
  const Float contour =
    CORE::ENVELOPE::tick(synth.envelope.gate, synth.envelope.envelope);
  const Float gated =
    CORE::ENVELOPE::tick(synth.gate.gate, synth.gate.envelope);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float pitch =
    key + synth.offset + synth.rows[BEND] + synth.rows[VIBRATO] * lfo;
  Float sum = ::mixed(synth, pitch, ::width(synth, lfo, contour));
  sum += synth.rows[NOISE] * noise;
  const Float cutoff = ::swept(synth, key, lfo, contour);
  CORE::FILTER::settle(
    synth.ladder, cutoff, synth.rows[RESONANCE], ::DRIVE, synth.rate);
  const Flag open = Whole(synth.rows[AMPLIFIER]) == OPEN;
  const Float out =
    CORE::FILTER::tick(synth.ladder, sum) * (open ? gated : contour);
  if (::quiet(synth)) note.sounding = false;
  return CORE::SHAPER::tick(synth.blocker, out);
}
