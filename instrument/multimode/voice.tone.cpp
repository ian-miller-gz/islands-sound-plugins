// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float SPAN = MULTIMODE::SQUARE - MULTIMODE::NARROW;
constexpr Float WIDE = 0.75f;
constexpr Float SLIGHT = 0.25f;
constexpr Float PANS[MULTIMODE::VOICES] = {-FULL, FULL, -WIDE,   WIDE,
                                           -HALF, HALF, -SLIGHT, SLIGHT};

auto pitch(const MULTIMODE::Synth &synth, Whole at, Float key, Float source)
  -> Float {
  const Float *rows = synth.rows;
  const Float cents = synth.allocator.notes[at].cents / CENT;
  return key + cents + MULTIMODE::stray(synth, at) + synth.tune +
         rows[MULTIMODE::BEND] + rows[MULTIMODE::MODULATION] * source;
}

auto swept(
  const MULTIMODE::Synth &synth, Whole at, Float key, Float contour,
  Float lfo) -> Float {
  const Float *rows = synth.rows;
  Float octaves =
    rows[MULTIMODE::TRACKING] * (key - MULTIMODE::REFERENCE) / OCTAVE;
  octaves +=
    rows[MULTIMODE::ENVELOPE] * contour + rows[MULTIMODE::WOBBLE] * lfo;
  octaves += rows[MULTIMODE::TOUCH] * synth.allocator.notes[at].velocity;
  octaves += MULTIMODE::open(synth, at);
  return rows[MULTIMODE::CUTOFF] * std::exp2(octaves);
}

void control(
  MULTIMODE::Synth &synth, Whole at, Float key, Float contour, Float lfo) {
  MULTIMODE::Voice &voice = synth.voices[at];
  if (!CORE::BLOCK::due(voice.stride, MULTIMODE::STRIDE)) return;
  MULTIMODE::drift(synth, voice);
  MULTIMODE::steer(synth, voice, ::swept(synth, at, key, contour, lfo));
}

auto play(MULTIMODE::Synth &synth, Whole at, Float lfo) -> Float {
  MULTIMODE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float *rows = synth.rows;
  const Float contour =
    CORE::ENVELOPE::tick(voice.contour.gate, voice.contour.envelope);
  const Float loud =
    CORE::ENVELOPE::tick(voice.loudness.gate, voice.loudness.envelope);
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  const Float source = rows[MULTIMODE::MODULATOR] > 0 ? contour : lfo;
  const Float pwm = rows[MULTIMODE::SOURCE] > 0 ? contour : (lfo + FULL) * HALF;
  const Float width =
    rows[MULTIMODE::WIDTH] - SPAN * rows[MULTIMODE::SWEEP] * pwm;
  ::control(synth, at, key, contour, lfo);
  const Float mixed =
    MULTIMODE::sing(synth, voice, ::pitch(synth, at, key, source), width);
  const Float out = MULTIMODE::filter(synth, voice, mixed) * loud;
  if (!CORE::ENVELOPE::sounding(voice.loudness.gate)) note.sounding = false;
  return CORE::SHAPER::tick(voice.blocker, out);
}

auto gain(Float pan) -> Float { return pan > 0 ? FULL - pan : FULL; }

}  // namespace

auto SOUND::PLUGINS::MULTIMODE::sound(Synth &synth) -> Pair {
  const Float lfo = CORE::MODULATOR::tick(synth.lfo);
  const Float spread = synth.rows[SPREAD];
  Pair pair;
  for (Whole at = 0; at < VOICES; ++at) {
    const Float out = ::play(synth, at, lfo);
    const Float pan = PANS[at] * spread;
    pair.left += out * ::gain(pan);
    pair.right += out * ::gain(-pan);
    pair.whole += out;
  }
  return pair;
}
