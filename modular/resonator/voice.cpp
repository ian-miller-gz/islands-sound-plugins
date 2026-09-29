// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<RESONATOR::Module, RESONATOR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new RESONATOR::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(RESONATOR::SHEET, module->rows);
  module->filters.assign(channels, CORE::FILTER::Variable{});
  module->lfo.wave = CORE::MODULATOR::SINE;
  RESONATOR::settle(*module);
  RESONATOR::sweep(*module, 0);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<RESONATOR::Module *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<RESONATOR::Module *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = RESONATOR::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::AUDIO}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "resonator", .type = "modular", .surface = &surface});

}  // namespace
