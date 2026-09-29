// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float TURNS = 2;
constexpr Float DEPTH = TURNS * CORE::OSCILLATOR::TAU;
constexpr Float CENT = 100;
constexpr Float LEVEL = 0.25f;
constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;

auto own(const OPERATOR::Routing &routing, Whole unit) -> Flag {
  return routing.into == routing.from && routing.into == OPERATOR::bit(unit);
}

void tune(OPERATOR::Synth &synth, OPERATOR::Voice &voice, Float key) {
  const Float pitch =
    key + synth.tune + synth.rows[OPERATOR::BEND] + synth.vibrato;
  const Float hertz = CORE::PHASE::hertz(pitch);
  for (Whole unit = 0; unit < OPERATOR::OPERATORS; ++unit) {
    const Float loop = ::own(*synth.routing, unit) ? synth.feedback : 0;
    CORE::OSCILLATOR::settle(
      voice.units[unit], hertz * synth.factors[unit], loop, synth.rate);
  }
  voice.stale = false;
}

auto modulated(
  const OPERATOR::Synth &synth, const OPERATOR::Voice &voice,
  Whole unit) -> Float {
  const OPERATOR::Routing &routing = *synth.routing;
  Float sum = 0;
  for (Whole from = unit + 1; from < OPERATOR::OPERATORS; ++from)
    if ((routing.inputs[unit] & OPERATOR::bit(from)) != 0)
      sum += voice.outs[from];
  Float phase = sum * DEPTH;
  if (!::own(routing, unit) && routing.into == OPERATOR::bit(unit))
    phase += synth.feedback * voice.outs[std::countr_zero(routing.from)];
  return phase;
}

auto play(OPERATOR::Synth &synth, Whole at) -> Float {
  OPERATOR::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float key = CORE::VOICE::tick(note, synth.allocator.glide);
  if (voice.stale || synth.clock == 0) ::tune(synth, voice, key);
  const Whole carriers = synth.routing->carriers;
  Float out = 0;
  Flag alive = false;
  for (Whole unit = OPERATOR::OPERATORS; unit-- > 0;) {
    const Float level =
      synth.levels[unit] *
      CORE::ENVELOPE::tick(voice.walks[unit], synth.segments[unit]);
    const Float phase = ::modulated(synth, voice, unit);
    voice.outs[unit] =
      CORE::OSCILLATOR::tick(voice.units[unit], synth.table, phase, level);
    if ((carriers & OPERATOR::bit(unit)) == 0) continue;
    out += voice.outs[unit];
    alive = alive || CORE::ENVELOPE::sounding(voice.walks[unit]);
  }
  if (!alive) note.sounding = false;
  return out;
}

}  // namespace

auto SOUND::OPERATOR::sound(Synth &synth) -> Float {
  const Float wave = synth.sign * CORE::MODULATOR::tick(synth.lfo);
  if (synth.clock == 0) synth.vibrato = wave * synth.rows[PITCH] / ::CENT;
  const Float swell = synth.rows[AMPLITUDE] * (wave + ::FULL) * ::HALF;
  Float out = 0;
  for (Whole at = 0; at < VOICES; ++at) out += ::play(synth, at);
  synth.clock = (synth.clock + 1) % CONTROL;
  return out * (::FULL - swell) * ::LEVEL;
}
