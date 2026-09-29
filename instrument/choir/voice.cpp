// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<CHOIR::Choir, CHOIR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *choir = new CHOIR::Choir{.rate = rate, .channels = channels};
  CHOIR::build(*choir);
  CORE::TABLE::rest(CHOIR::SHEET, choir->rows);
  CHOIR::settle(*choir);
  return choir;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<CHOIR::Choir *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<CHOIR::Choir *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = CHOIR::render,
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
  PLUGIN::offer({.name = "choir", .surface = &surface});

}  // namespace
