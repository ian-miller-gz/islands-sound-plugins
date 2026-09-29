// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<LOFI::Effect, LOFI::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new LOFI::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(LOFI::SHEET, effect->rows);
  LOFI::build(*effect);
  LOFI::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<LOFI::Effect *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<LOFI::Effect *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = LOFI::render,
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
  PLUGIN::offer({.name = "lofi", .surface = &surface});

}  // namespace
