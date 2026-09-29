// SPDX-License-Identifier: AGPL-3.0-or-later
#include <numbers>

#include "voice.internal.hpp"

namespace {

constexpr Float TAU = 2.0f * std::numbers::pi_v<Float>;

}  // namespace

auto SOUND::PLUGINS::VIBRATO::excursion(Float hertz, Float cents) -> Float {
  if (hertz <= 0) return 0;
  return (CORE::PHASE::ratio(cents) - UNITY) / (::TAU * hertz);
}

void SOUND::PLUGINS::VIBRATO::settle(Effect &effect) {
  effect.lfo.hertz = effect.rows[RATE];
  CORE::MODULATOR::settle(effect.lfo, effect.rate);
  const Float swing = excursion(effect.rows[RATE], effect.rows[DEPTH]);
  for (Channel &strip : effect.strips)
    CORE::LINE::settle(strip.sweep, MARGIN + swing, swing, GLIDE, effect.rate);
}

void SOUND::PLUGINS::VIBRATO::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
