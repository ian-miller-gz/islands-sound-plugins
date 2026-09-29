// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::GATE::build(Effect &effect) {
  effect.keys.resize(effect.channels);
  effect.sense.kind = CORE::DYNAMICS::PEAK;
  effect.sense.release = SENSE;
  CORE::DYNAMICS::settle(effect.sense, effect.rate);
  effect.envelope.kind = CORE::DYNAMICS::PEAK;
}

void SOUND::PLUGINS::GATE::settle(Effect &effect) {
  for (auto &key : effect.keys) {
    key.hertz = effect.rows[SIDECHAIN];
    CORE::DYNAMICS::settle(key, effect.rate);
  }
  effect.hold.time = effect.rows[HOLD] * MILLI;
  CORE::DYNAMICS::settle(effect.hold, effect.rate);
  effect.envelope.attack = effect.rows[ATTACK] * MILLI;
  effect.envelope.release = effect.rows[RELEASE] * MILLI;
  CORE::DYNAMICS::settle(effect.envelope, effect.rate);
  effect.threshold = CORE::DYNAMICS::gain(effect.rows[THRESHOLD]);
  effect.floor = CORE::DYNAMICS::gain(effect.rows[RANGE]);
}
