// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole KINDS[RESONATOR::MODES] = {
  CORE::FILTER::LOW, CORE::FILTER::BAND, CORE::FILTER::HIGH,
  CORE::FILTER::NOTCH};

auto kind(const RESONATOR::Module &module) -> Whole {
  const Whole mode = Whole(module.rows[RESONATOR::MODE]);
  return KINDS[mode < RESONATOR::MODES ? mode : RESONATOR::LOW];
}

}  // namespace

void SOUND::PLUGINS::RESONATOR::settle(Module &module) {
  module.lfo.hertz = module.rows[RATE];
  CORE::MODULATOR::settle(module.lfo, module.rate);
  module.countdown = 0;
}

void SOUND::PLUGINS::RESONATOR::sweep(Module &module, Float swing) {
  const Float *rows = module.rows;
  const Float hertz = rows[CUTOFF] * std::exp2(swing * rows[DEPTH]);
  const Whole chosen = ::kind(module);
  for (CORE::FILTER::Variable &filter : module.filters)
    CORE::FILTER::settle(filter, chosen, hertz, rows[EMPHASIS], module.rate);
}

void SOUND::PLUGINS::RESONATOR::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
