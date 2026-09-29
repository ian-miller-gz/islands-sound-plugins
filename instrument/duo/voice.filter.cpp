// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float OCTAVE = 12;
constexpr Float DRIVE = 1;

auto two(DUO::Synth &synth, Float in, Float cutoff) -> Float {
  CORE::FILTER::settle(
    synth.variable, CORE::FILTER::LOW, cutoff, synth.rows[DUO::RESONANCE],
    synth.rate);
  return CORE::FILTER::tick(synth.variable, in);
}

auto four(DUO::Synth &synth, Float in, Float cutoff) -> Float {
  CORE::FILTER::settle(
    synth.ladder, cutoff, synth.rows[DUO::RESONANCE], DRIVE, synth.rate);
  return CORE::FILTER::tick(synth.ladder, in);
}

using Pass = auto (*)(DUO::Synth &, Float, Float) -> Float;

constexpr Pass PASSES[] = {two, four};
static_assert(std::size(PASSES) == std::size(DUO::SLOPES));

}  // namespace

auto SOUND::DUO::filter(
  Synth &synth, Float in, Float key, Float lfo, Float random,
  Float contour) -> Float {
  const Float *rows = synth.rows;
  Float octaves = rows[TRACKING] * (key - REFERENCE) / OCTAVE;
  octaves += rows[AMOUNT] * contour;
  octaves += rows[WOBBLE] * lfo + rows[SCATTER] * random;
  const Float cutoff = rows[CUTOFF] * std::exp2(octaves);
  const Float passed = CORE::FILTER::tick(synth.highpass, in);
  return PASSES[Whole(rows[SLOPE])](synth, passed, cutoff);
}
