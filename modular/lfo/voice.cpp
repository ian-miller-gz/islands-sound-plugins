// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<LFO::Module, LFO::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new LFO::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(LFO::SHEET, module->rows);
  LFO::settle(*module);
  CORE::MODULATOR::reset(module->lfo);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<LFO::Module *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<LFO::Module *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = LFO::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO, "cv"}}};

[[maybe_unused]] const Flag offered =
  SOUND::PLUGIN::offer({.name = "lfo", .type = "modular", .surface = &surface});

}  // namespace
