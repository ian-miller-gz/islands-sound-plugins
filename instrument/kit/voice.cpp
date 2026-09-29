// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<KIT::Kit, KIT::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *kit = new KIT::Kit{.rate = rate, .channels = channels};
  CORE::TABLE::rest(KIT::SHEET, kit->rows);
  KIT::settle(*kit);
  return kit;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<KIT::Kit *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<KIT::Kit *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = KIT::render,
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
  {.name = "kit",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
