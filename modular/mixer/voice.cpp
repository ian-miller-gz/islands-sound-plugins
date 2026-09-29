// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<MIXER::Module, MIXER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new MIXER::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(MIXER::SHEET, module->rows);
  MIXER::settle(*module);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<MIXER::Module *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<MIXER::Module *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = MIXER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins =
    {{AUDIO::PLUGIN::Port::AUDIO, "1"},
     {AUDIO::PLUGIN::Port::AUDIO, "2"},
     {AUDIO::PLUGIN::Port::AUDIO, "3"},
     {AUDIO::PLUGIN::Port::AUDIO, "4"},
     {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "mixer", .type = "modular", .surface = &surface});

}  // namespace
