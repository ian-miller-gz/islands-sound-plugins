// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::EXPANDER::build(Effect &effect) {
  effect.detector.kind = CORE::DYNAMICS::PEAK;
  effect.computer.side = CORE::DYNAMICS::BELOW;
}

void SOUND::EXPANDER::settle(Effect &effect) {
  effect.detector.attack = effect.rows[ATTACK] * MILLI;
  effect.detector.release = effect.rows[RELEASE] * MILLI;
  CORE::DYNAMICS::settle(effect.detector, effect.rate);
  effect.computer.threshold = effect.rows[THRESHOLD];
  effect.computer.ratio = effect.rows[RATIO];
  effect.computer.knee = effect.rows[KNEE];
  CORE::DYNAMICS::settle(effect.computer);
}
