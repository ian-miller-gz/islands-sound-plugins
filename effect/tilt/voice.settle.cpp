// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

void SOUND::PLUGINS::TILT::build(Effect &effect) {
  effect.poles.resize(effect.channels);
}

void SOUND::PLUGINS::TILT::settle(Effect &effect) {
  for (auto &pole : effect.poles)
    CORE::FILTER::settle(
      pole, CORE::FILTER::LOW, effect.rows[PIVOT], effect.rate);
  const Float level = CORE::DYNAMICS::gain(effect.rows[GAIN]);
  const Float lean = effect.rows[TILT] * HALF;
  effect.lows = level * CORE::DYNAMICS::gain(-lean);
  effect.highs = level * CORE::DYNAMICS::gain(lean);
}
