// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<WIDENER::Effect, WIDENER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new WIDENER::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(WIDENER::SHEET, effect->rows);
  WIDENER::build(*effect);
  WIDENER::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<WIDENER::Effect *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<WIDENER::Effect *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = WIDENER::render,
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
  {.name = "widener", .type = "effect", .surface = &surface});

}  // namespace

void SOUND::PLUGINS::WIDENER::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
