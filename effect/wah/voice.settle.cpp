// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void tune(const CORE::FILTER::Variable &from, CORE::FILTER::Variable &into) {
  into.damping = from.damping;
  std::copy(
    std::begin(from.weights), std::end(from.weights), std::begin(into.weights));
  std::copy(std::begin(from.mix), std::end(from.mix), std::begin(into.mix));
}

void shape(WAH::Effect &effect, Float centre) {
  CORE::FILTER::settle(
    effect.shape, CORE::FILTER::BAND, centre, effect.rows[WAH::EMPHASIS],
    effect.rate);
  for (CORE::FILTER::Variable &strip : effect.strips)
    ::tune(effect.shape, strip);
}

}  // namespace

void SOUND::PLUGINS::WAH::sweep(Effect &effect, Float level) {
  const Float reach = std::min(level * effect.gain, UNITY);
  ::shape(effect, HEEL * std::exp(reach * effect.span));
}

void SOUND::PLUGINS::WAH::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  effect.detector.kind = CORE::DYNAMICS::PEAK;
}

void SOUND::PLUGINS::WAH::settle(Effect &effect) {
  effect.detector.attack = effect.rows[ATTACK] * MILLI;
  effect.detector.release = effect.rows[RELEASE] * MILLI;
  CORE::DYNAMICS::settle(effect.detector, effect.rate);
  effect.gain = CORE::DYNAMICS::gain(effect.rows[SENSITIVITY]);
  effect.span = std::log(effect.rows[RANGE] / HEEL);
  ::shape(effect, HEEL);
  effect.trim = std::sqrt(effect.shape.damping);
}
