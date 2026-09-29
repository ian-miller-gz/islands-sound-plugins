// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SHIFTER::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  for (CORE::PITCH::Grains &grains : effect.strips)
    CORE::PITCH::build(grains, WIDEST / SECOND, effect.rate);
}

void SOUND::PLUGINS::SHIFTER::settle(Effect &effect) {
  const Float cents = effect.rows[SHIFT] * SEMITONE + effect.rows[FINE];
  const Float seconds = effect.rows[WINDOW] / SECOND;
  for (CORE::PITCH::Grains &grains : effect.strips)
    CORE::PITCH::settle(grains, cents, seconds, effect.rate);
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}

void SOUND::PLUGINS::SHIFTER::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
