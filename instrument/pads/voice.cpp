// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<PADS::Kit, PADS::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *kit = new PADS::Kit{.rate = rate, .channels = channels};
  CORE::TABLE::rest(PADS::SHEET, kit->rows);
  PADS::settle(*kit);
  return kit;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<PADS::Kit *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<PADS::Kit *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = PADS::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  PLUGIN::offer({.name = "pads", .surface = &surface});

}  // namespace
