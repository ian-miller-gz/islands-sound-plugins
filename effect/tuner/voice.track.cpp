// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Integer REACH = 6;
constexpr Integer KEYS = Integer(TUNER::KEYS);
constexpr Float FAR = 1000.0f;

auto member(const TUNER::Tuner &tuner, Integer note) -> Flag {
  const Whole scale = Whole(tuner.rows[TUNER::SCALE]);
  const Integer root = Integer(tuner.rows[TUNER::ROOT]);
  const Integer degree = ((note - root) % ::KEYS + ::KEYS) % ::KEYS;
  const Whole mask = TUNER::MASKS[scale < TUNER::SCALES ? scale : 0];
  return ((mask >> Whole(degree)) & 1u) != 0;
}

auto nearest(const TUNER::Tuner &tuner, Float pitch) -> Integer {
  const Integer centre = Integer(std::lround(pitch));
  Integer best = centre;
  Float gap = ::FAR;
  for (Integer offset = -::REACH; offset <= ::REACH; ++offset) {
    const Integer note = centre + offset;
    const Float distance = std::fabs(Float(note) - pitch);
    if (!::member(tuner, note) || distance >= gap) continue;
    best = note;
    gap = distance;
  }
  return best;
}

auto pitched(Float hertz) -> Float {
  return CORE::PHASE::ANCHOR +
         CORE::PHASE::OCTAVE * std::log2(hertz / CORE::PHASE::CONCERT);
}

}  // namespace

void SOUND::PLUGINS::TUNER::follow(Tuner &tuner, Float sample) {
  if (!CORE::PITCH::feed(tuner.detector, &sample, 1)) return;
  const CORE::PITCH::Estimate &estimate = tuner.detector.estimate;
  if (estimate.confidence < VOICED || estimate.hertz <= 0) {
    tuner.note = NONE;
    tuner.target = 0;
    retime(tuner, false);
    return;
  }
  const Float pitch = ::pitched(estimate.hertz);
  const Integer note = ::nearest(tuner, pitch);
  const Flag held = note == tuner.note;
  tuner.note = note;
  tuner.target = (Float(note) - pitch) * CENT;
  retime(tuner, held);
}
