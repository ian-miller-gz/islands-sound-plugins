// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::NOISE::seed(Module &module) {
  module.sources.assign(module.channels, Source{});
  CORE::NOISE::Register state = CORE::NOISE::SEED;
  for (Source &source : module.sources) {
    CORE::NOISE::seed(source.white, state);
    CORE::NOISE::seed(source.pink, state);
    CORE::NOISE::seed(source.brown, state);
    CORE::NOISE::seed(source.blue, state);
    state += SPACING;
  }
}

void SOUND::PLUGINS::NOISE::settle(Module &module) {
  module.level.time = SMOOTH;
  CORE::MODULATOR::settle(module.level, module.rate);
  module.target = module.rows[GAIN] * module.rows[GATE];
}

void SOUND::PLUGINS::NOISE::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
