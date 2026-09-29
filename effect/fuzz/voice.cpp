// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<FUZZ::Effect, FUZZ::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new FUZZ::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(FUZZ::SHEET, effect->rows);
  FUZZ::build(*effect);
  FUZZ::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<FUZZ::Effect *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<FUZZ::Effect *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = FUZZ::render,
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
  SOUND::PLUGIN::offer({.name = "fuzz", .type = "effect", .surface = &surface});

}  // namespace

void SOUND::PLUGINS::FUZZ::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
