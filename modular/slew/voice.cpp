// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<SLEW::Module, SLEW::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new SLEW::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(SLEW::SHEET, module->rows);
  module->slews.assign(channels, CORE::MODULATOR::Slew{});
  SLEW::settle(*module);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<SLEW::Module *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<SLEW::Module *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = SLEW::render,
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
  {.name = "slew", .type = "modular", .surface = &surface});

}  // namespace
