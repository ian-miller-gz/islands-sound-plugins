// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float HALF = 0.5f;

}  // namespace

void SOUND::DEESSER::build(Deesser &deesser) {
  deesser.filters.resize(deesser.channels);
  deesser.bands.assign(deesser.channels, 0);
  deesser.detector.kind = CORE::DYNAMICS::PEAK;
  deesser.computer.side = CORE::DYNAMICS::ABOVE;
  deesser.computer.knee = KNEE;
}

void SOUND::DEESSER::settle(Deesser &deesser) {
  for (CORE::FILTER::Biquad &filter : deesser.filters)
    CORE::FILTER::settle(
      filter, CORE::FILTER::HIGH, deesser.rows[FREQUENCY], CORE::FILTER::FLAT,
      0, deesser.rate);
  deesser.detector.attack = deesser.rows[ATTACK] / MILLI;
  deesser.detector.release = deesser.rows[RELEASE] / MILLI;
  CORE::DYNAMICS::settle(deesser.detector, deesser.rate);
  deesser.computer.threshold = deesser.rows[THRESHOLD];
  deesser.computer.ratio = deesser.rows[RATIO];
  CORE::DYNAMICS::settle(deesser.computer);
  deesser.mode = deesser.rows[MODE] >= ::HALF ? WIDE : BAND;
  deesser.listen = deesser.rows[LISTEN] >= ::HALF;
}

void SOUND::DEESSER::apply(
  Deesser &deesser, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  deesser.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(deesser);
}

auto SOUND::DEESSER::reduced(Deesser &deesser, Float band) -> Float {
  const Float level = CORE::DYNAMICS::tick(deesser.detector, band);
  const Float heard = CORE::DYNAMICS::decibels(level);
  const Float cut = CORE::DYNAMICS::reduce(deesser.computer, heard);
  const Float floor = -deesser.rows[RANGE];
  return CORE::DYNAMICS::gain(cut < floor ? floor : cut);
}
