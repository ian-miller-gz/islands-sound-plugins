// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<CHORUS::Effect, CHORUS::SHEET>;

auto reach(Whole rate) -> Whole {
  const Float deepest = CORE::TABLE::found(CHORUS::SHEET, CHORUS::DEPTH)->most *
                        CHORUS::MILLISECOND;
  return Whole(std::ceil((CHORUS::CENTRE + deepest) * Float(rate)));
}

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new CHORUS::Effect;
  effect->rate = rate;
  effect->channels = channels;
  effect->strips.resize(channels);
  for (CHORUS::Channel &strip : effect->strips)
    CORE::LINE::build(strip.line, ::reach(rate));
  CORE::TABLE::rest(CHORUS::SHEET, effect->rows);
  CHORUS::settle(*effect);
  CHORUS::place(*effect);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<CHORUS::Effect *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<CHORUS::Effect *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = CHORUS::render,
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
  PLUGIN::offer({.name = "chorus", .surface = &surface});

}  // namespace

void SOUND::CHORUS::apply(Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
  if (event.index == VOICES || event.index == SPREAD) place(effect);
}
