// SPDX-License-Identifier: AGPL-3.0-or-later
#include "filter.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float FEEDBACK = 17.0f;
constexpr Float SCALES[] = {1.0f, 0.5f, 0.5f, 0.5f};
static_assert(sizeof(SCALES) / sizeof(SCALES[0]) == FILTER::STAGES);

auto spill(const FILTER::Diode &diode, Whole at) -> Float {
  return diode.betas[at] *
         (diode.states[at] + diode.feedbacks[at] * diode.deltas[at]);
}

}  // namespace

void SOUND::CORE::FILTER::settle(
  Diode &diode, Float cutoff, Float emphasis, Float drive, Whole rate) {
  const Float g = warped(cutoff, rate);
  Float above = 0, product = ONE;
  for (Whole at = STAGES; at-- > 0;) {
    const Float edge = SCALES[at] * g;
    diode.betas[at] = ONE / (ONE + g - edge * above);
    const Float gain = edge * diode.betas[at];
    diode.gammas[at] = ONE + gain * above;
    diode.epsilons[at] = above;
    diode.deltas[at] = at + 1 < STAGES ? edge : 0;
    diode.sums[at] = product;
    product *= gain;
    above = gain;
  }
  diode.alpha = g / (ONE + g);
  diode.feedback = bounded(emphasis) * FEEDBACK;
  diode.scale = ONE / (ONE + diode.feedback * product);
  diode.drive = drive;
}

auto SOUND::CORE::FILTER::tick(Diode &diode, Float in) -> Float {
  diode.feedbacks[STAGES - 1] = 0;
  for (Whole at = STAGES - 1; at-- > 0;)
    diode.feedbacks[at] = ::spill(diode, at + 1);
  Float sum = 0;
  for (Whole at = 0; at < STAGES; ++at)
    sum += diode.sums[at] * ::spill(diode, at);
  Float signal = diode.drive * in - diode.feedback * sum;
  signal = SHAPER::soft(signal * diode.scale);
  for (Whole at = 0; at < STAGES; ++at) {
    signal = signal * diode.gammas[at] + diode.feedbacks[at] +
             diode.epsilons[at] * ::spill(diode, at);
    const Float step = (SCALES[at] * signal - diode.states[at]) * diode.alpha;
    signal = step + diode.states[at];
    diode.states[at] = signal + step;
  }
  return signal;
}
