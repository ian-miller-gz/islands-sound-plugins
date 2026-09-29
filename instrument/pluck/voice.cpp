// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<PLUCK::Synth, PLUCK::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new PLUCK::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(PLUCK::SHEET, synth->rows);
  PLUCK::build(*synth);
  PLUCK::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<PLUCK::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<PLUCK::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = PLUCK::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "pluck",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
