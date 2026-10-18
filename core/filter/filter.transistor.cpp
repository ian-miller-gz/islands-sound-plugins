// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float TURN = 6.28318531f;
constexpr Float EDGE = 0.25f;
constexpr Float EXCESS = 1.05f;
constexpr Float TUNING[] = {1.0f, -1.31f, 7.2f, -7.0f};
constexpr Float LOOP[] = {1.0f, 3.45f, -12.0f, -11.6f};

template <Whole Count>
auto series(const Float (&terms)[Count], Float ratio) -> Float {
  Float sum = 0;
  for (Whole at = Count; at-- > 0;) sum = sum * ratio + terms[at];
  return sum;
}

}  // namespace

void SOUND::PLUGINS::CORE::FILTER::settle(
  Transistor &transistor, Float cutoff, Float emphasis, Float drive, Float bass,
  Whole rate) {
  const Float ratio = bounded(cutoff, rate) / Float(rate);
  const Float edge = ratio < EDGE ? ratio : EDGE;
  const Float loop = RING * ::series(LOOP, edge) * EXCESS;
  transistor.gain = (ONE - std::exp(-TURN * ratio)) * ::series(TUNING, edge);
  transistor.feedback = loop * bounded(emphasis);
  transistor.drive = drive < ONE ? ONE : drive;
  transistor.lift = ONE + bounded(bass) * transistor.feedback;
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Transistor &transistor, Float in)
  -> Float {
  Float *states = transistor.states;
  const Float fed = (states[POLES - 1] + transistor.last) * HALF;
  transistor.last = states[POLES - 1];
  const Float driven = transistor.drive * transistor.lift * in;
  Float signal = SHAPER::soft(driven - transistor.feedback * fed);
  for (Whole at = 0; at < POLES; ++at) {
    const Float step = SHAPER::soft(signal) - SHAPER::soft(states[at]);
    states[at] += transistor.gain * step;
    signal = states[at];
  }
  return signal;
}
