// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MILLISECOND = 0.001f;
constexpr Float SPAN = 12;
constexpr Float CENTRE = 5;
constexpr Float SWING = 1.6f;
constexpr Float TREMBLE = 0.25f;
constexpr Float BUCKET = 8000;
constexpr Float HALF = 0.5f;
constexpr Float THIRD = 1.0f / 3.0f;
constexpr Float SIDE = 2.0f / 3.0f;
constexpr Float FULL = 1;
constexpr Whole LEFT = 0;
constexpr Whole MIDDLE = 1;
constexpr Whole RIGHT = 2;
static_assert(RIGHT + 1 == ENSEMBLE::LINES);

void start(CORE::MODULATOR::Lfo &lfo, Whole at) {
  lfo.wave = CORE::MODULATOR::SINE;
  lfo.start = Float(at) * THIRD;
  CORE::MODULATOR::reset(lfo);
}

}  // namespace

void SOUND::PLUGINS::ENSEMBLE::build(Ensemble &ensemble, Whole rate) {
  CORE::LINE::build(ensemble.line, Whole(SPAN * MILLISECOND * Float(rate)));
  for (Whole at = 0; at < LINES; ++at) {
    ::start(ensemble.slow[at], at);
    ::start(ensemble.fast[at], at);
  }
}

void SOUND::PLUGINS::ENSEMBLE::settle(
  Ensemble &ensemble, const Float *rows, Whole rate) {
  CORE::FILTER::settle(ensemble.bucket, CORE::FILTER::LOW, BUCKET, rate);
  for (Whole at = 0; at < LINES; ++at) {
    ensemble.slow[at].hertz = rows[CHORUS];
    ensemble.fast[at].hertz = rows[VIBRATO];
    CORE::MODULATOR::settle(ensemble.slow[at], rate);
    CORE::MODULATOR::settle(ensemble.fast[at], rate);
    CORE::LINE::settle(
      ensemble.sweeps[at], CENTRE * MILLISECOND, MILLISECOND, 0, rate);
  }
  ensemble.wet = rows[MIX];
  ensemble.chorus = rows[DEPTH] * SWING;
  ensemble.vibrato = rows[DEPTH] * TREMBLE;
}

auto SOUND::PLUGINS::ENSEMBLE::tick(Ensemble &ensemble, Float in) -> Pair {
  Float wets[LINES];
  for (Whole at = 0; at < LINES; ++at) {
    const Float swing =
      ensemble.chorus * CORE::MODULATOR::tick(ensemble.slow[at]) +
      ensemble.vibrato * CORE::MODULATOR::tick(ensemble.fast[at]);
    wets[at] = CORE::LINE::read(ensemble.line, ensemble.sweeps[at], swing);
  }
  CORE::LINE::write(ensemble.line, CORE::FILTER::tick(ensemble.bucket, in));
  const Float wet = ensemble.wet;
  const Float dry = (FULL - wet) * in;
  const Float middle = HALF * wets[MIDDLE];
  const Float sum = wets[LEFT] + wets[MIDDLE] + wets[RIGHT];
  return {
    dry + wet * SIDE * (wets[LEFT] + middle),
    dry + wet * SIDE * (wets[RIGHT] + middle), dry + wet * THIRD * sum};
}
