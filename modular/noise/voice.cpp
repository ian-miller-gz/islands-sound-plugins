// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<NOISE::Module, NOISE::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new NOISE::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(NOISE::SHEET, module->rows);
  NOISE::seed(*module);
  NOISE::settle(*module);
  CORE::MODULATOR::jump(module->level, module->target);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<NOISE::Module *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<NOISE::Module *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = NOISE::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "noise", .type = "modular", .surface = &surface});

}  // namespace
