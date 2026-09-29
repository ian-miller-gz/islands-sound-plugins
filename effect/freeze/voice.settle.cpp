// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds / FREEZE::SECOND * Float(rate);
}

}  // namespace

void SOUND::FREEZE::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  const Float reach =
    OVERLAP * ::frames(LONGEST, effect.rate) + CORE::LINE::NEAREST;
  for (Strip &strip : effect.strips) {
    CORE::LINE::build(strip.line, Whole(std::ceil(reach)));
    CORE::CLOUD::build(strip.cloud, GRAINS);
  }
  effect.latch.time = SMOOTHING;
  CORE::MODULATOR::settle(effect.latch, effect.rate);
}

void SOUND::FREEZE::settle(Effect &effect) {
  const Float size = ::frames(effect.rows[SIZE], effect.rate);
  effect.grain.span = OVERLAP * size;
  effect.grain.step = -UNITY;
  effect.grain.gain = UNITY;
  effect.grain.delay = CORE::LINE::NEAREST + effect.grain.span;
  effect.interval = size;
  if (effect.countdown > effect.interval) effect.countdown = effect.interval;
  const Float tail = effect.rows[DECAY] * Float(effect.rate);
  effect.fade = std::pow(SILENCE, UNITY / tail);
  effect.mix = effect.rows[MIX];
  effect.held = effect.rows[HOLD] >= HALF;
}

void SOUND::FREEZE::engage(Effect &effect) {
  for (Strip &strip : effect.strips) CORE::CLOUD::clear(strip.cloud);
  effect.level = UNITY;
  effect.countdown = 0;
}

void SOUND::FREEZE::apply(Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  const Flag was = effect.held;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
  if (effect.held && !was) engage(effect);
}
