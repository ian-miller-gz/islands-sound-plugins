// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float BASE = 4;
constexpr Float MIDDLE = 60;
constexpr Float OCTAVE = 12;
constexpr Float TOP = 2;

auto taper(const TINE::Synth &synth, Float key) -> Float {
  const Float factor =
    std::exp2(-synth.rows[TINE::TAPER] * (key - MIDDLE) / OCTAVE);
  return factor < TOP ? factor : TOP;
}

}  // namespace

auto SOUND::PLUGINS::TINE::curve(const Synth &synth, Float velocity) -> Float {
  return std::pow(velocity, std::pow(::BASE, -synth.rows[CURVE]));
}

void SOUND::PLUGINS::TINE::touch(Synth &synth, Whole at) {
  const Float key = Float(synth.allocator.notes[at].pitch);
  synth.voices[at].taper = ::taper(synth, key);
}
