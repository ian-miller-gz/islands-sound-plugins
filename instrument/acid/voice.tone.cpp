// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1;
constexpr Float EMPHASIS = 0.9f;
constexpr Float PUSH = 2;
constexpr Float BOOST = 1;
constexpr Float FULL = 1;
constexpr Whole STRIDE = 16;

auto doubled(const ACID::Synth &synth) -> Whole {
  return synth.rate * ACID::OVERSAMPLING;
}

struct Model {
  void (*settle)(ACID::Synth &synth, Float cutoff);
  auto (*tick)(ACID::Synth &synth, Float in) -> Float;
};

constexpr Model MODELS[] = {
  {[](ACID::Synth &synth, Float cutoff) {
     const Float emphasis = EMPHASIS * synth.rows[ACID::RESONANCE];
     CORE::FILTER::settle(
       synth.diode, cutoff, emphasis, UNITY, ::doubled(synth));
   },
   [](ACID::Synth &synth, Float in) {
     return CORE::FILTER::tick(synth.diode, in);
   }},
  {[](ACID::Synth &synth, Float cutoff) {
     ACID::settle(
       synth.bassline, cutoff, synth.rows[ACID::RESONANCE], ::doubled(synth));
   },
   [](ACID::Synth &synth, Float in) {
     return ACID::tick(synth.bassline, in);
   }}};
static_assert(std::size(MODELS) == std::size(ACID::FILTERS));

auto oscillate(ACID::Synth &synth, Float key) -> Float {
  const Float hertz = CORE::PHASE::hertz(key + synth.rows[ACID::TUNE]);
  CORE::OSCILLATOR::settle(
    synth.oscillator, hertz, CORE::OSCILLATOR::SQUARE, synth.rate);
  const Float wave = CORE::OSCILLATOR::tick(synth.oscillator, synth.wave);
  return CORE::FILTER::tick(synth.coupling, wave);
}

void steer(ACID::Synth &synth, Float contour, Float push) {
  const Float *rows = synth.rows;
  const Float octaves = rows[ACID::ENVELOPE] * contour + PUSH * push;
  MODELS[synth.filter].settle(synth, rows[ACID::CUTOFF] * std::exp2(octaves));
}

auto pass(ACID::Synth &synth, Float in) -> Float {
  const auto tick = MODELS[synth.filter].tick;
  const auto shape = [&synth, tick](Float sample) {
    return synth.level * CORE::SHAPER::soft(synth.drive * tick(synth, sample));
  };
  return CORE::SHAPER::tick(synth.oversampler, in, shape);
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
  if (CORE::BLOCK::due(synth.stride, ::STRIDE)) ::steer(synth, contour, push);
  const Float wave = ::oscillate(synth, key);
  const Float out = ::pass(synth, wave) * loud * (::FULL + ::BOOST * accent);
  if (!CORE::ENVELOPE::sounding(synth.door)) note.sounding = false;
  return CORE::SHAPER::tick(synth.blocker, out);
}
