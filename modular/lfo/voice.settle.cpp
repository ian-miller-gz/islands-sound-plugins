// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FACTORS[] = {1.0f, LFO::FACTOR};
static_assert(std::size(FACTORS) == std::size(LFO::RANGES));

}  // namespace

void SOUND::PLUGINS::LFO::tune(Module &module) {
  const Float *rows = module.rows;
  CORE::MODULATOR::Lfo &lfo = module.lfo;
  const Float factor = ::FACTORS[Whole(rows[RANGE])];
  lfo.hertz = CORE::BLOCK::scaled(
    rows[RATE] * factor, module.cv, rows[AMOUNT], SLOWEST * factor,
    FASTEST * factor);
  lfo.slew = rows[SMOOTH] / lfo.hertz;
  CORE::MODULATOR::settle(lfo, module.rate);
}

void SOUND::PLUGINS::LFO::settle(Module &module) {
  const Float *rows = module.rows;
  CORE::MODULATOR::Lfo &lfo = module.lfo;
  lfo.wave = Whole(rows[WAVE]);
  lfo.depth = rows[DEPTH];
  lfo.start = rows[PHASE];
  lfo.fade = rows[FADE];
  lfo.skew = rows[SKEW];
  tune(module);
}

void SOUND::PLUGINS::LFO::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
  if (event.index == PHASE || event.index == FADE)
    CORE::MODULATOR::reset(module.lfo);
}
