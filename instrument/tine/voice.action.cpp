// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float BASE = 4;
constexpr Float MIDDLE = 60;
constexpr Float OCTAVE = 12;
constexpr Float TOP = 2;
constexpr Float ABOVE = 6;
constexpr Float FULL = 1;

auto taper(const TINE::Synth &synth, Float key) -> Float {
  const Float factor =
    std::exp2(-synth.rows[TINE::TAPER] * (key - MIDDLE) / OCTAVE);
  return factor < TOP ? factor : TOP;
}

void darken(TINE::Synth &synth, TINE::Voice &voice) {
  const Float closed = synth.rows[TINE::TOUCH] * (FULL - voice.velocity);
  const Float cutoff = voice.hertz * std::exp2(ABOVE - closed);
  CORE::FILTER::settle(voice.low, CORE::FILTER::LOW, cutoff, synth.rate);
}

}  // namespace

auto SOUND::PLUGINS::TINE::curve(const Synth &synth, Float velocity) -> Float {
  return std::pow(velocity, std::pow(::BASE, -synth.rows[CURVE]));
}

void SOUND::PLUGINS::TINE::touch(Synth &synth, Whole at) {
  const Float key = Float(synth.allocator.notes[at].pitch);
  TINE::Voice &voice = synth.voices[at];
  voice.taper = ::taper(synth, key);
  ::darken(synth, voice);
}
