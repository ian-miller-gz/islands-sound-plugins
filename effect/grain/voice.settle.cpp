// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto reach(Whole rate) -> Whole {
  const Float climb = CORE::PHASE::ratio(GRAIN::OCTAVES * GRAIN::SEMITONE);
  const Float span = GRAIN::WIDEST + (climb - GRAIN::UNITY) * GRAIN::LONGEST;
  return Whole(std::ceil(span / GRAIN::SECOND * Float(rate)));
}

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds / GRAIN::SECOND * Float(rate);
}

}  // namespace

void SOUND::PLUGINS::GRAIN::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
  for (Strip &strip : effect.strips) {
    CORE::LINE::build(strip.line, ::reach(effect.rate));
    CORE::CLOUD::build(strip.cloud, GRAINS);
    strip.seed = seed;
    CORE::MODULATOR::draw(seed);
  }
}

void SOUND::PLUGINS::GRAIN::settle(Effect &effect) {
  const Float span = ::frames(effect.rows[SIZE], effect.rate);
  const Float ratio = CORE::PHASE::ratio(effect.rows[PITCH] * SEMITONE);
  const Float overlap = effect.rows[DENSITY] * effect.rows[SIZE] / SECOND;
  effect.grain.span = span;
  effect.grain.step = UNITY - ratio;
  effect.grain.gain = UNITY / std::sqrt(overlap > UNITY ? overlap : UNITY);
  effect.lift = ratio > UNITY ? (ratio - UNITY) * span : 0;
  effect.spray = ::frames(effect.rows[SPRAY], effect.rate);
  effect.interval = Float(effect.rate) / effect.rows[DENSITY];
  if (effect.countdown > effect.interval) effect.countdown = effect.interval;
  effect.feedback = effect.rows[FEEDBACK];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}

void SOUND::PLUGINS::GRAIN::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
