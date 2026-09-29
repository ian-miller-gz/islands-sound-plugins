// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<DOUBLER::Doubler, DOUBLER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *doubler = new DOUBLER::Doubler{.rate = rate, .channels = channels};
  DOUBLER::build(*doubler);
  CORE::TABLE::rest(DOUBLER::SHEET, doubler->rows);
  DOUBLER::settle(*doubler);
  return doubler;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<DOUBLER::Doubler *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<DOUBLER::Doubler *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = DOUBLER::render,
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
  PLUGIN::offer({.name = "doubler", .surface = &surface});

}  // namespace
