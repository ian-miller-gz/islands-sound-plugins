// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float PI = 3.14159265f;
constexpr Float CENT = 100;
constexpr Float STEP = 1;
constexpr Float DOWN = -1;
constexpr Float UP = 1;

constexpr CORE::MODULATOR::Wave SHAPES[] = {
  CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SAW,  CORE::MODULATOR::SAW,
  CORE::MODULATOR::SQUARE,   CORE::MODULATOR::SINE, CORE::MODULATOR::SAMPLE};
constexpr Float SIGNS[] = {UP, DOWN, UP, UP, UP, UP};
static_assert(std::size(SHAPES) == std::size(OPERATOR::WAVES));
static_assert(std::size(SIGNS) == std::size(SHAPES));

auto looped(Float feedback) -> Float {
  if (feedback <= 0) return 0;
  return PI * std::exp2(feedback - OPERATOR::LOOPS);
}

void prepare(OPERATOR::Synth &synth, Whole unit) {
  const Float *rows = synth.rows;
  CORE::ENVELOPE::Segments &segments = synth.segments[unit];
  for (Whole stage = 0; stage < OPERATOR::STAGES; ++stage) {
    segments.rates[stage] = rows[OPERATOR::field(unit, OPERATOR::RATE + stage)];
    segments.levels[stage] =
      rows[OPERATOR::field(unit, OPERATOR::LEVEL + stage)];
  }
  segments.curve = CORE::ENVELOPE::EXPONENTIAL;
  segments.depth = rows[OPERATOR::VELOCITY];
  CORE::ENVELOPE::shape(segments, synth.rate);
  const Float detune = rows[OPERATOR::field(unit, OPERATOR::DETUNE)] * STEP;
  synth.factors[unit] =
    rows[OPERATOR::field(unit, OPERATOR::RATIO)] * CORE::PHASE::ratio(detune);
  synth.levels[unit] = CORE::ENVELOPE::loudness(
    rows[OPERATOR::field(unit, OPERATOR::OUTPUT)] / OPERATOR::TOP,
    CORE::ENVELOPE::EXPONENTIAL);
}

void swing(OPERATOR::Synth &synth) {
  const Whole wave = Whole(synth.rows[OPERATOR::WAVE]);
  const Whole at = wave < std::size(SHAPES) ? wave : 0;
  synth.lfo.wave = SHAPES[at];
  synth.sign = SIGNS[at];
  synth.lfo.hertz = synth.rows[OPERATOR::SPEED];
  synth.lfo.fade = synth.rows[OPERATOR::DELAY];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::OPERATOR::settle(Synth &synth) {
  synth.routing = &routed(synth.rows[ALGORITHM]);
  synth.feedback = ::looped(synth.rows[FEEDBACK]);
  for (Whole unit = 0; unit < OPERATORS; ++unit) ::prepare(synth, unit);
  ::swing(synth);
  CORE::VOICE::settle(synth.allocator, synth.rate);
  synth.tune = synth.rows[TUNE] / CENT;
  for (Voice &voice : synth.voices) voice.stale = true;
}
