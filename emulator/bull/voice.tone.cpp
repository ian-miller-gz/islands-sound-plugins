// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float OPEN = 1;

auto sing(
  CORE::OSCILLATOR::Oscillator &oscillator, Float hertz, Whole wave,
  Whole rate) -> Float {
  CORE::OSCILLATOR::settle(oscillator, hertz, CORE::OSCILLATOR::SQUARE, rate);
  return CORE::OSCILLATOR::tick(oscillator, wave);
}

auto mixed(BULL::Synth &synth, Float pitch) -> Float {
  const Float hertz = CORE::PHASE::hertz(pitch);
  const Whole rate = synth.rate;
  Float sum =
    sing(synth.oscillators[BULL::FIRST], hertz, synth.waves[BULL::FIRST], rate);
  sum += sing(
    synth.oscillators[BULL::SECOND], hertz * synth.beat,
    synth.waves[BULL::SECOND], rate);
  sum += synth.chain[BULL::SUB] *
         sing(synth.sub, hertz * HALF, CORE::OSCILLATOR::PULSE, rate);
  return sum * HALF;
}

auto swept(const BULL::Synth &synth, Float contour) -> Float {
  const Float octaves = synth.chain[BULL::CONTOUR] * contour;
  return synth.chain[BULL::CUTOFF] * std::exp2(octaves);
}

}  // namespace

auto SOUND::PLUGINS::BULL::sound(Synth &synth) -> Float {
  CORE::VOICE::Note &note = synth.allocator.notes[0];
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float sum = ::mixed(synth, key + synth.offset);
  const Float contour =
    CORE::ENVELOPE::tick(synth.filter.gate, synth.filter.envelope);
  const Float loud =
    CORE::ENVELOPE::tick(synth.loudness.gate, synth.loudness.envelope);
  CORE::FILTER::settle(
    synth.ladder, ::swept(synth, contour), synth.chain[EMPHASIS], ::OPEN,
    synth.rate);
  const Float out = CORE::FILTER::tick(synth.ladder, sum) * loud;
  if (!CORE::ENVELOPE::sounding(synth.loudness.gate)) note.sounding = false;
  const Float blocked = CORE::SHAPER::tick(synth.blocker, out);
  return CORE::SHAPER::soft(blocked * synth.chain[DRIVE]);
}
