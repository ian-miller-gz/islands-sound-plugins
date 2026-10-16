// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto mode(const RESONATOR::Module &module) -> Whole {
  const Whole mode = Whole(module.rows[RESONATOR::MODE]);
  return mode < RESONATOR::MODES ? mode : RESONATOR::LOW;
}

}  // namespace

void SOUND::PLUGINS::RESONATOR::settle(Module &module) {
  module.lfo.hertz = module.rows[RATE];
  CORE::MODULATOR::settle(module.lfo, module.rate);
  CORE::BLOCK::reset(module.stride);
  const Whole model = chosen(module);
  if (model == module.model) return;
  for (Stage &stage : module.stages) stage = Stage{};
  module.model = model;
}

void SOUND::PLUGINS::RESONATOR::sweep(
  Module &module, Whole channel, Float swing, Float cv) {
  const Float *rows = module.rows;
  const Tone tone = {
    .kind = ::mode(module),
    .cutoff = CORE::BLOCK::scaled(
      rows[CUTOFF] * std::exp2(swing), cv, rows[DEPTH], LOWEST, HIGHEST),
    .emphasis = rows[EMPHASIS],
    .drive = rows[DRIVE],
    .rate = module.rate * TWICE};
  model(module).settle(module.stages[channel], tone);
}

void SOUND::PLUGINS::RESONATOR::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
