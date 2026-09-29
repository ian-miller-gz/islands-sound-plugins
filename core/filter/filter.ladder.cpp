// SPDX-License-Identifier: AGPL-3.0-or-later
#include "filter.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float FEEDBACK = 4.2f;

}  // namespace

void SOUND::CORE::FILTER::settle(
  Ladder &ladder, Float cutoff, Float emphasis, Float drive, Whole rate) {
  for (Pole &pole : ladder.poles) settle(pole, LOW, cutoff, rate);
  const Float gain = ladder.poles[0].gain;
  Float weight = ONE - gain;
  for (Whole at = POLES; at-- > 0;) {
    ladder.weights[at] = weight;
    weight *= gain;
  }
  const Float whole = weight / (ONE - gain);
  ladder.feedback = bounded(emphasis) * FEEDBACK;
  ladder.scale = ONE / (ONE + ladder.feedback * whole);
  ladder.drive = drive;
}

auto SOUND::CORE::FILTER::tick(Ladder &ladder, Float in) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < POLES; ++at)
    sum += ladder.weights[at] * ladder.poles[at].state;
  Float signal = ladder.drive * in - ladder.feedback * sum;
  signal = SHAPER::soft(signal * ladder.scale);
  for (Pole &pole : ladder.poles) signal = tick(pole, signal);
  return signal;
}
