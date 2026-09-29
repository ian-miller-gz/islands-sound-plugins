// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<MACHINE::Machine, MACHINE::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *machine = new MACHINE::Machine{.rate = rate, .channels = channels};
  CORE::TABLE::rest(MACHINE::SHEET, machine->rows);
  CORE::PERCUSSION::HYBRID::build(machine->hat, rate);
  MACHINE::settle(*machine);
  return machine;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<MACHINE::Machine *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<MACHINE::Machine *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = MACHINE::render,
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
  PLUGIN::offer({.name = "machine", .surface = &surface});

}  // namespace
