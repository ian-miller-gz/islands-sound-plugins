// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<TUNER::Tuner, TUNER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *tuner = new TUNER::Tuner{.rate = rate, .channels = channels};
  TUNER::build(*tuner);
  CORE::TABLE::rest(TUNER::SHEET, tuner->rows);
  TUNER::settle(*tuner);
  TUNER::steer(*tuner);
  return tuner;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<TUNER::Tuner *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<TUNER::Tuner *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = TUNER::render,
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
  {.name = "tuner", .type = "effect", .surface = &surface});

}  // namespace
