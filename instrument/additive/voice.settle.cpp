// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SINGLE = 1.0f;
constexpr Float SILENT = 0.000001f;

auto delta(Float span, Whole rate) -> Float {
  const Float frames = span * Float(rate);
  return frames <= SINGLE ? SINGLE : SINGLE / frames;
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::settle(Synth &synth) {
  const Float *rows = synth.rows;
  synth.rise = ::delta(rows[ATTACK], synth.rate);
  synth.fall = ::delta(rows[DECAY], synth.rate) * (SINGLE - rows[SUSTAIN]);
  synth.drop = ::delta(rows[RELEASE], synth.rate);
  synth.span = 0;
  for (Whole at = 0; at < HARMONICS; ++at) synth.span += rows[HARMONIC1 + at];
  if (synth.span < ::SILENT) synth.span = SINGLE;
}
