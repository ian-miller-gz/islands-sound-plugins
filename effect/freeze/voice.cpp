// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<FREEZE::Effect, FREEZE::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new FREEZE::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(FREEZE::SHEET, effect->rows);
  FREEZE::build(*effect);
  FREEZE::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<FREEZE::Effect *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<FREEZE::Effect *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = FREEZE::render,
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
  PLUGIN::offer({.name = "freeze", .surface = &surface});

}  // namespace
