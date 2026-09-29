// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<PARAMETRIC::Effect, PARAMETRIC::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new PARAMETRIC::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(PARAMETRIC::SHEET, effect->rows);
  PARAMETRIC::build(*effect);
  PARAMETRIC::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<PARAMETRIC::Effect *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<PARAMETRIC::Effect *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = PARAMETRIC::render,
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
  {.name = "parametric", .type = "effect", .surface = &surface});

}  // namespace

void SOUND::PLUGINS::PARAMETRIC::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
