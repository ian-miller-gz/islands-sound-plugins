// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<BUS::Bus, BUS::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *bus = new BUS::Bus{.rate = rate, .channels = channels};
  CORE::TABLE::rest(BUS::SHEET, bus->rows);
  BUS::build(*bus);
  BUS::settle(*bus);
  return bus;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<BUS::Bus *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<BUS::Bus *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = BUS::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::AUDIO}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  SOUND::PLUGIN::offer({.name = "bus", .type = "effect", .surface = &surface});

}  // namespace
