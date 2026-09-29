// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float DRONE = -60;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float SPAN = DUO::SQUARE - DUO::NARROW;

auto noise(DUO::Synth &synth) -> Float {
  if (synth.rows[DUO::COLOUR] > 0) return CORE::NOISE::tick(synth.noise);
  return CORE::NOISE::tick(synth.noise.white);
}

auto swing(DUO::Synth &synth) -> Float {
  const CORE::PHASE::Wheel before = synth.lfo.phase;
  const Float value = CORE::MODULATOR::tick(synth.lfo);
  if (synth.lfo.phase >= before) return value;
  if (synth.rows[DUO::CLOCK] == 0) synth.held = synth.input;
  const CORE::VOICE::Note &lead = synth.low.notes[0];
  if (synth.rows[DUO::REPEAT] > 0 && lead.held)
    DUO::strike(synth, lead.velocity);
  return value;
}

auto width(const DUO::Synth &synth, Float lfo, Float adsr) -> Float {
  const Float depth = synth.rows[DUO::SOURCE] > 0 ? adsr : (lfo + FULL) * HALF;
  return synth.rows[DUO::WIDTH] - SPAN * synth.rows[DUO::SWEEP] * depth;
}

void quiet(DUO::Synth &synth) {
  if (CORE::ENVELOPE::sounding(synth.adsr.gate)) return;
  if (CORE::ENVELOPE::sounding(synth.ar.gate)) return;
  synth.low.notes[0].sounding = false;
  synth.high.notes[0].sounding = false;
}

}  // namespace

auto SOUND::PLUGINS::DUO::sound(Synth &synth) -> Float {
  const Float *rows = synth.rows;
  const Float noise = ::noise(synth);
  const Float lfo = ::swing(synth);
  const Float random = CORE::MODULATOR::tick(synth.lag, synth.held);
  const Float adsr = CORE::ENVELOPE::tick(synth.adsr.gate, synth.adsr.envelope);
  const Float ar = CORE::ENVELOPE::tick(synth.ar.gate, synth.ar.envelope);
  const Float low = CORE::VOICE::tick(synth.low.notes[0], synth.low.glide);
  const Float high = CORE::VOICE::tick(synth.high.notes[0], synth.high.glide);
  const Float drift =
    synth.tune + rows[BEND] + rows[VIBRATO] * lfo + rows[SAMPLE] * random;
  const Float root = rows[KEYBOARD] > 0 ? low : REFERENCE + ::DRONE;
  const Tones tones = sing(
    synth, root + rows[COARSE1] + drift,
    high + rows[COARSE2] + synth.fine + drift, ::width(synth, lfo, adsr));
  synth.input = tones.saw + (noise - tones.saw) * rows[MIX];
  const Float sum = tones.mixed + rows[NOISE] * noise;
  const Float contour = rows[CONTOUR] > 0 ? ar : adsr;
  const Float filtered = filter(synth, sum, low, lfo, random, contour);
  const Float gain = rows[AMPLIFIER] > 0 ? ar : adsr;
  ::quiet(synth);
  return CORE::SHAPER::tick(synth.blocker, filtered * gain);
}
