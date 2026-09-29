// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<RINGMOD::Effect, RINGMOD::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new RINGMOD::Effect;
  effect->rate = rate;
  effect->channels = channels;
  CORE::TABLE::rest(RINGMOD::SHEET, effect->rows);
  CORE::OSCILLATOR::build(effect->table);
  RINGMOD::settle(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<RINGMOD::Effect *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<RINGMOD::Effect *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = RINGMOD::render,
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
  {.name = "ringmod", .type = "effect", .surface = &surface});

}  // namespace
