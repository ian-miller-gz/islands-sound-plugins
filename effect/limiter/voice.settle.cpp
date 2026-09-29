// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::LIMITER::build(Effect &effect) {
  const Whole ahead = Whole(AHEAD * Float(effect.rate));
  effect.rings.clear();
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    effect.rings.push_back(CORE::DYNAMICS::RING::create(ahead));
    CORE::DYNAMICS::delay(effect.rings.back(), ahead);
  }
  effect.hold.time = AHEAD;
  CORE::DYNAMICS::settle(effect.hold, effect.rate);
  effect.detector.kind = CORE::DYNAMICS::PEAK;
  effect.detector.attack = AHEAD * RAMP;
}

void SOUND::LIMITER::settle(Effect &effect) {
  effect.detector.release = effect.rows[RELEASE] * MILLI;
  CORE::DYNAMICS::settle(effect.detector, effect.rate);
  effect.ceiling = CORE::DYNAMICS::gain(effect.rows[CEILING]);
  effect.gain = CORE::DYNAMICS::gain(effect.rows[GAIN]);
}
