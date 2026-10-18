// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float PI = 3.14159265f;
constexpr Float CENT = 100;
constexpr Float STEP = 1;
constexpr Float DOWN = -1;
constexpr Float UP = 1;
constexpr Float HALF = 0.5f;
constexpr Float HERTZ = 100;

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
  const Float detune = rows[OPERATOR::field(unit, OPERATOR::DETUNE)] * STEP;
  const Float factor =
    rows[OPERATOR::field(unit, OPERATOR::RATIO)] * CORE::PHASE::ratio(detune);
  const Flag fixed = rows[OPERATOR::trait(unit, OPERATOR::FIXED)] > HALF;
  const Float hertz = factor * HERTZ * CORE::PHASE::ratio(rows[OPERATOR::TUNE]);
  synth.factors[unit] = fixed ? 0 : factor;
  synth.fixed[unit] = fixed ? hertz : 0;
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

void SOUND::PLUGINS::OPERATOR::settle(Synth &synth) {
  synth.routing = &routed(synth.rows[ALGORITHM]);
  synth.feedback = ::looped(synth.rows[FEEDBACK]);
  for (Whole unit = 0; unit < OPERATORS; ++unit) ::prepare(synth, unit);
  for (Whole at = 0; at < VOICES; ++at) fit(synth, at);
  trace(synth);
  ::swing(synth);
  allocate(synth);
  synth.tune = synth.rows[TUNE] / CENT;
  for (Voice &voice : synth.voices) voice.stale = true;
}
