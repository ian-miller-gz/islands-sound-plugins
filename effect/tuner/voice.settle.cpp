// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float MILLI = 1000.0f;

}  // namespace

void SOUND::TUNER::build(Tuner &tuner) {
  CORE::PITCH::build(tuner.detector, tuner.rate, LOWEST, HIGHEST, HOP);
  tuner.shifters.resize(tuner.channels);
  for (Shifter &shifter : tuner.shifters) {
    CORE::PITCH::build(shifter.grains, WINDOW, tuner.rate);
    CORE::PITCH::build(shifter.formant, WINDOW, tuner.rate);
  }
}

void SOUND::TUNER::retime(Tuner &tuner, Flag held) {
  const Float base = tuner.rows[RETUNE] / ::MILLI;
  const Float stretch =
    held ? ::UNITY + tuner.rows[HUMANISE] * HUMANE : ::UNITY;
  tuner.correction.time = base * stretch;
  CORE::MODULATOR::settle(tuner.correction, tuner.rate / STRIDE);
}

void SOUND::TUNER::settle(Tuner &tuner) {
  tuner.keep = tuner.rows[FORMANT] >= ::HALF;
  retime(tuner, false);
}

void SOUND::TUNER::apply(Tuner &tuner, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  tuner.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(tuner);
}

void SOUND::TUNER::steer(Tuner &tuner) {
  const Float correction =
    CORE::MODULATOR::tick(tuner.correction, tuner.target);
  const Float cents = correction + tuner.rows[SHIFT] * CENT;
  for (Shifter &shifter : tuner.shifters) {
    if (tuner.keep)
      CORE::PITCH::settle(shifter.formant, cents, 0, WINDOW, tuner.rate);
    else
      CORE::PITCH::settle(shifter.grains, cents, WINDOW, tuner.rate);
  }
}
