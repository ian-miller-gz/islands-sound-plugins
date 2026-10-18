// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using CORE::OSCILLATOR::Wheel;

constexpr Float PERIODS = 1;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;
constexpr Float LIGHT = 0.25f;
constexpr Wheel MIDWAY = Wheel(CORE::PHASE::HALF);

auto period(PLUCK::Voice &voice, Float value) -> Float {
  PLUCK::Exciter &exciter = voice.exciter;
  if (exciter.left == 0) return 0;
  --exciter.left;
  exciter.wave.phase += exciter.wave.step;
  exciter.low += voice.burst.colour * (value - exciter.low);
  return exciter.low * exciter.level;
}

auto noise(const PLUCK::Synth &, PLUCK::Voice &voice) -> Float {
  return CORE::NOISE::tick(voice.burst);
}

auto pick(const PLUCK::Synth &, PLUCK::Voice &voice) -> Float {
  return period(voice, CORE::OSCILLATOR::saw(voice.exciter.wave));
}

auto mallet(const PLUCK::Synth &synth, PLUCK::Voice &voice) -> Float {
  const Wheel phase = voice.exciter.wave.phase;
  return period(voice, CORE::OSCILLATOR::sine(synth.table, phase));
}

using Excite = auto (*)(const PLUCK::Synth &, PLUCK::Voice &) -> Float;

struct Shape {
  Excite excite;
  Float pace;
  Wheel start;
  Float gain;
};

constexpr Shape SHAPES[] = {
  {noise, FULL, 0, FULL}, {pick, FULL, MIDWAY, LIGHT}, {mallet, HALF, 0, FULL}};
static_assert(std::size(SHAPES) == PLUCK::KINDS + 1);

}  // namespace

void SOUND::PLUGINS::PLUCK::strike(Synth &synth, Whole at) {
  Voice &voice = synth.voices[at];
  const CORE::VOICE::Note &note = synth.allocator.notes[at];
  const Float hertz = CORE::PHASE::hertz(Float(note.pitch) + synth.tune);
  const Float colour = synth.rows[TONE] * (HALF + HALF * note.velocity);
  CORE::NOISE::settle(voice.burst, PERIODS / hertz, colour, synth.rate);
  CORE::NOISE::strike(voice.burst, note.velocity);
  const Whole kind = Whole(synth.rows[EXCITER]);
  const ::Shape &shape = ::SHAPES[kind <= KINDS ? kind : 0];
  Exciter &exciter = voice.exciter;
  const Whole frames = Whole(Float(synth.rate) / hertz);
  const Float level = note.velocity * shape.gain;
  exciter = {.kind = kind, .left = frames, .level = level};
  CORE::OSCILLATOR::settle(
    exciter.wave, hertz * shape.pace, CORE::OSCILLATOR::SQUARE, synth.rate);
  CORE::OSCILLATOR::reset(exciter.wave, shape.start);
}

auto SOUND::PLUGINS::PLUCK::excite(const Synth &synth, Voice &voice) -> Float {
  const Whole kind = voice.exciter.kind;
  return ::SHAPES[kind <= KINDS ? kind : 0].excite(synth, voice);
}
