// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<AMPLIFIER::Module, AMPLIFIER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new AMPLIFIER::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(AMPLIFIER::SHEET, module->rows);
  module->filters.resize(channels);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<AMPLIFIER::Module *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<AMPLIFIER::Module *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = AMPLIFIER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins =
    {{AUDIO::PLUGIN::Port::AUDIO, "in"},
     {AUDIO::PLUGIN::Port::AUDIO, "cv"},
     {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "amplifier", .type = "modular", .surface = &surface});

}  // namespace
