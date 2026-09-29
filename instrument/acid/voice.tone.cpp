// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float DRIVE = 1;
constexpr Float EMPHASIS = 0.9f;
constexpr Float PUSH = 2;
constexpr Float BOOST = 1;
constexpr Float FULL = 1;

auto oscillate(ACID::Synth &synth, Float key) -> Float {
  const Float hertz = CORE::PHASE::hertz(key + synth.rows[ACID::TUNE]);
  CORE::OSCILLATOR::settle(
    synth.oscillator, hertz, CORE::OSCILLATOR::SQUARE, synth.rate);
  const Float wave = CORE::OSCILLATOR::tick(synth.oscillator, synth.wave);
  return CORE::FILTER::tick(synth.coupling, wave);
}

auto filter(ACID::Synth &synth, Float in, Float contour, Float push) -> Float {
  const Float *rows = synth.rows;
  const Float octaves = rows[ACID::ENVELOPE] * contour + PUSH * push;
  const Float cutoff = rows[ACID::CUTOFF] * std::exp2(octaves);
  CORE::FILTER::settle(
    synth.diode, cutoff, EMPHASIS * rows[ACID::RESONANCE], DRIVE, synth.rate);
  return CORE::FILTER::tick(synth.diode, in);
}

}  // namespace

auto SOUND::PLUGINS::ACID::sound(Synth &synth) -> Float {
  CORE::VOICE::Note &note = synth.allocator.notes[0];
  const Float contour =
    CORE::ENVELOPE::tick(synth.contour, synth.contours[synth.accent]);
  const Float loud = CORE::ENVELOPE::tick(synth.door, synth.amplifier);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float accent = synth.accent == STRONG ? synth.rows[ACCENT] : 0;
  const Float push = CORE::MODULATOR::tick(synth.push, accent * contour);
  const Float wave = ::oscillate(synth, key);
  const Float out =
    ::filter(synth, wave, contour, push) * loud * (FULL + BOOST * accent);
  if (!CORE::ENVELOPE::sounding(synth.door)) note.sounding = false;
  return CORE::SHAPER::tick(synth.blocker, out);
}
