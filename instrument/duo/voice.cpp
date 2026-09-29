// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<DUO::Synth, DUO::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new DUO::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(DUO::SHEET, synth->rows);
  DUO::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<DUO::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<DUO::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = DUO::render,
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
  {.name = "duo",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::MONO,
   .surface = &surface});

}  // namespace
