// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float VIBRATO = 2;
constexpr Float WOBBLE = 2;
constexpr Float OCTAVE = 12;

auto noise(LADDER::Synth &synth) -> Float {
  if (synth.rows[LADDER::COLOUR] > 0) return CORE::NOISE::tick(synth.noise);
  return CORE::NOISE::tick(synth.noise.white);
}

auto swept(const LADDER::Synth &synth, Float key, Float wheel, Float contour)
  -> Float {
  Float octaves = synth.tracking * (key - LADDER::REFERENCE) / OCTAVE;
  octaves += synth.rows[LADDER::AMOUNT] * contour;
  if (synth.rows[LADDER::FILTER] > 0) octaves += wheel * WOBBLE;
  return synth.rows[LADDER::CUTOFF] * std::exp2(octaves);
}

auto mixed(LADDER::Synth &synth, Float bent, Float wheel, Float noise)
  -> Float {
  const Float vibrato = synth.rows[LADDER::PITCH] > 0 ? wheel * VIBRATO : 0;
  Float sum = synth.rows[LADDER::NOISE] * noise;
  for (Whole at = LADDER::FIRST; at < LADDER::THIRD; ++at) {
    LADDER::Source &source = synth.sources[at];
    sum += source.level * LADDER::sing(source, bent + vibrato, synth.rate);
  }
  return sum;
}

}  // namespace

auto SOUND::PLUGINS::LADDER::sound(Synth &synth) -> Float {
  CORE::VOICE::Note &note = synth.allocator.notes[0];
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide) + synth.tune;
  const Float bent = key + synth.rows[BEND];
  const Float noise = ::noise(synth);
  const Flag keyed = synth.rows[KEYBOARD] > 0;
  Source &control = synth.sources[THIRD];
  const Float third =
    sing(control, keyed ? bent : REFERENCE + synth.tune, synth.rate);
  const Float mix = synth.rows[MIX];
  const Float wheel = synth.rows[MODULATION] * (third + (noise - third) * mix);
  const Float sum = ::mixed(synth, bent, wheel, noise) + control.level * third;
  const Float contour =
    CORE::ENVELOPE::tick(synth.filter.gate, synth.filter.envelope);
  const Float loud =
    CORE::ENVELOPE::tick(synth.loudness.gate, synth.loudness.envelope);
  const Float cutoff = ::swept(synth, key, wheel, contour);
  CORE::FILTER::settle(
    synth.ladder, cutoff, synth.rows[EMPHASIS], synth.rows[DRIVE], synth.rate);
  const Float out = CORE::FILTER::tick(synth.ladder, sum) * loud;
  if (!CORE::ENVELOPE::sounding(synth.loudness.gate)) note.sounding = false;
  return CORE::SHAPER::tick(synth.blocker, out);
}
