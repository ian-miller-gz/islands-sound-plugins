// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<PHASER::Effect, PHASER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *effect = new PHASER::Effect;
  effect->rate = rate;
  effect->channels = channels;
  effect->strips.resize(channels);
  effect->lfo.wave = CORE::MODULATOR::SINE;
  CORE::TABLE::rest(PHASER::SHEET, effect->rows);
  PHASER::settle(*effect);
  CORE::MODULATOR::reset(effect->lfo);
  return effect;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<PHASER::Effect *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<PHASER::Effect *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = PHASER::render,
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
  PLUGIN::offer({.name = "phaser", .surface = &surface});

}  // namespace

void SOUND::PHASER::apply(Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}

void SOUND::PHASER::settle(Effect &effect) {
  effect.stages = FEWEST + STRIDE * Whole(effect.rows[STAGES]);
  effect.swing = effect.rows[DEPTH] * OCTAVES;
  effect.lfo.hertz = effect.rows[RATE];
  CORE::MODULATOR::settle(effect.lfo, effect.rate);
}
