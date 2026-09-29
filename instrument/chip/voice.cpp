// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<CHIP::Synth, CHIP::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new CHIP::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(CHIP::SHEET, synth->rows);
  CHIP::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<CHIP::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<CHIP::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = CHIP::render,
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
  {.name = "chip",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::MONO,
   .surface = &surface});

}  // namespace
