// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<OSCILLATOR::Module, OSCILLATOR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new OSCILLATOR::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(OSCILLATOR::SHEET, module->rows);
  CORE::OSCILLATOR::build(module->table);
  OSCILLATOR::seat(*module);
  OSCILLATOR::settle(*module);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<OSCILLATOR::Module *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<OSCILLATOR::Module *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = OSCILLATOR::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  PLUGIN::offer({.name = "oscillator", .surface = &surface});

}  // namespace
