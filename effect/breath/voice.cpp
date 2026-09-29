// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<BREATH::Breath, BREATH::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *breath = new BREATH::Breath{.rate = rate, .channels = channels};
  BREATH::build(*breath);
  CORE::TABLE::rest(BREATH::SHEET, breath->rows);
  BREATH::settle(*breath);
  return breath;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<BREATH::Breath *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<BREATH::Breath *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = BREATH::render,
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
  {.name = "breath", .type = "effect", .surface = &surface});

}  // namespace
