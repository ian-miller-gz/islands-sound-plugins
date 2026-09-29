// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float UNITY = 1.0f;

void sense(CORE::DYNAMICS::Detector &detector, Whole rate) {
  detector.kind = CORE::DYNAMICS::RMS;
  detector.attack = BREATH::SENSE;
  detector.release = BREATH::FADE;
  CORE::DYNAMICS::settle(detector, rate);
}

}  // namespace

void SOUND::BREATH::build(Breath &breath) {
  breath.filters.resize(breath.channels);
  breath.bands.assign(breath.channels, 0);
  ::sense(breath.whole, breath.rate);
  ::sense(breath.band, breath.rate);
  breath.gate.kind = CORE::DYNAMICS::PEAK;
  breath.gate.level = ::UNITY;
}

void SOUND::BREATH::settle(Breath &breath) {
  for (CORE::FILTER::Biquad &filter : breath.filters)
    CORE::FILTER::settle(
      filter, CORE::FILTER::HIGH, breath.rows[FREQUENCY], CORE::FILTER::FLAT, 0,
      breath.rate);
  breath.gate.attack = breath.rows[ATTACK] / MILLI;
  breath.gate.release = breath.rows[RELEASE] / MILLI;
  CORE::DYNAMICS::settle(breath.gate, breath.rate);
  breath.threshold = CORE::DYNAMICS::gain(breath.rows[THRESHOLD]);
  breath.floor = CORE::DYNAMICS::gain(-breath.rows[REDUCTION]);
}

void SOUND::BREATH::apply(Breath &breath, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  breath.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(breath);
}

auto SOUND::BREATH::gated(Breath &breath, Float in, Float high) -> Float {
  const Float whole = CORE::DYNAMICS::tick(breath.whole, in);
  const Float band = CORE::DYNAMICS::tick(breath.band, high);
  const Flag noisy = band >= whole * LIKENESS;
  const Flag quiet = whole < breath.threshold;
  const Float open =
    CORE::DYNAMICS::tick(breath.gate, noisy && quiet ? 0 : ::UNITY);
  return breath.floor + (::UNITY - breath.floor) * open;
}
