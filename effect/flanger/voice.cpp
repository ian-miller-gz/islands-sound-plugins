// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<FLANGER::Effect, FLANGER::SHEET>;

auto reach(Whole rate) -> Whole {
  const Float manual =
    CORE::TABLE::found(FLANGER::SHEET, FLANGER::MANUAL)->most;
  const Float depth = CORE::TABLE::found(FLANGER::SHEET, FLANGER::DEPTH)->most;
  const Float seconds = (manual + depth) * FLANGER::MILLISECOND;
  return Whole(std::ceil(seconds * Float(rate)));
}

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new FLANGER::Effect;
  effect->rate = rate;
  effect->channels = channels;
  effect->strips.resize(channels);
  for (FLANGER::Channel &strip : effect->strips)
    CORE::LINE::build(strip.line, ::reach(rate));
  effect->lfo.wave = CORE::MODULATOR::TRIANGLE;
  CORE::TABLE::rest(FLANGER::SHEET, effect->rows);
  FLANGER::settle(*effect);
  CORE::MODULATOR::reset(effect->lfo);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<FLANGER::Effect *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<FLANGER::Effect *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = FLANGER::render,
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
  {.name = "flanger", .type = "effect", .surface = &surface});

}  // namespace

void SOUND::PLUGINS::FLANGER::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
