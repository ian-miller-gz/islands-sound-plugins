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
    CORE::NOISE::seed(source.violet, state);
    CORE::NOISE::seed(source.velvet, state);
    CORE::NOISE::seed(source.walk, state);
    state += SPACING;
  }
}

void SOUND::PLUGINS::NOISE::tune(Module &module) {
  const Float *rows = module.rows;
  const Float hertz =
    CORE::BLOCK::scaled(rows[RATE], module.cv, rows[AMOUNT], SLOWEST, FASTEST);
  for (Source &source : module.sources) {
    CORE::NOISE::settle(source.velvet, hertz, module.rate);
    CORE::NOISE::settle(source.hold, hertz, module.rate);
    CORE::NOISE::settle(source.walk, hertz, REACH, module.rate);
  }
}

void SOUND::PLUGINS::NOISE::settle(Module &module) {
  module.level.time = SMOOTH;
  CORE::MODULATOR::settle(module.level, module.rate);
  module.target = module.rows[GAIN] * module.rows[GATE];
  tune(module);
}

void SOUND::PLUGINS::NOISE::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
  if (event.index != MODE) return;
  for (Source &source : module.sources) CORE::NOISE::reset(source.hold);
}
