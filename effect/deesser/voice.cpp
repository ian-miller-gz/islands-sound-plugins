// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<DEESSER::Deesser, DEESSER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *deesser = new DEESSER::Deesser{.rate = rate, .channels = channels};
  DEESSER::build(*deesser);
  CORE::TABLE::rest(DEESSER::SHEET, deesser->rows);
  DEESSER::settle(*deesser);
  return deesser;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<DEESSER::Deesser *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<DEESSER::Deesser *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = DEESSER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::AUDIO}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "deesser", .type = "effect", .surface = &surface});

}  // namespace
