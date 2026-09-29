// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float HALF = 0.5f;
constexpr Float NARROWING = 0.45f;

}  // namespace

auto SOUND::DCO::sing(Synth &synth, Voice &voice, Float pitch, Float shape)
  -> Float {
  const Float *rows = synth.rows;
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Float width =
    CORE::OSCILLATOR::SQUARE - NARROWING * rows[WIDTH] * shape;
  CORE::OSCILLATOR::Oscillator &oscillator = voice.oscillator;
  CORE::OSCILLATOR::settle(oscillator, hertz, width, synth.rate);
  CORE::OSCILLATOR::settle(
    voice.sub, hertz * HALF, CORE::OSCILLATOR::SQUARE, synth.rate);
  const Float saw = CORE::OSCILLATOR::saw(oscillator);
  const Float pulse = CORE::OSCILLATOR::pulse(oscillator);
  oscillator.phase += oscillator.step;
  const Float sub = CORE::OSCILLATOR::tick(voice.sub, CORE::OSCILLATOR::PULSE);
  return rows[SAW] * saw + rows[PULSE] * pulse + rows[SUB] * sub;
}
