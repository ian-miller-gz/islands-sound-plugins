// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<VIBRATO::Effect, VIBRATO::SHEET>;

auto reach(Whole rate) -> Whole {
  const Float slowest =
    CORE::TABLE::found(VIBRATO::SHEET, VIBRATO::RATE)->least;
  const Float widest = CORE::TABLE::found(VIBRATO::SHEET, VIBRATO::DEPTH)->most;
  const Float swing = VIBRATO::excursion(slowest, widest);
  const Float seconds = VIBRATO::MARGIN + VIBRATO::DOUBLE * swing;
  return Whole(std::ceil(seconds * Float(rate)));
}

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new VIBRATO::Effect;
  effect->rate = rate;
  effect->channels = channels;
  effect->strips.resize(channels);
  for (VIBRATO::Channel &strip : effect->strips)
    CORE::LINE::build(strip.line, ::reach(rate));
  effect->lfo.wave = CORE::MODULATOR::SINE;
  CORE::TABLE::rest(VIBRATO::SHEET, effect->rows);
  VIBRATO::settle(*effect);
  CORE::MODULATOR::reset(effect->lfo);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<VIBRATO::Effect *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<VIBRATO::Effect *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = VIBRATO::render,
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
  {.name = "vibrato", .type = "effect", .surface = &surface});

}  // namespace
