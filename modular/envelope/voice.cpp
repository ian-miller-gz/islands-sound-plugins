// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<ENVELOPE::Module, ENVELOPE::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *module = new ENVELOPE::Module{.rate = rate, .channels = channels};
  CORE::TABLE::rest(ENVELOPE::SHEET, module->rows);
  ENVELOPE::seat(*module);
  ENVELOPE::settle(*module);
  return module;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<ENVELOPE::Module *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<ENVELOPE::Module *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = ENVELOPE::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO, "cv"}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "envelope",
   .type = "modular",
   .voicing = SOUND::PLUGIN::MONO,
   .surface = &surface});

}  // namespace
