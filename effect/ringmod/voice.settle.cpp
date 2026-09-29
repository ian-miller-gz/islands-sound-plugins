// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Carrier = auto (*)(const RINGMOD::Effect &effect) -> Float;

auto sine(const RINGMOD::Effect &effect) -> Float {
  return CORE::OSCILLATOR::sine(effect.table, effect.carrier.phase);
}

auto triangle(const RINGMOD::Effect &effect) -> Float {
  return CORE::OSCILLATOR::triangle(effect.carrier);
}

auto saw(const RINGMOD::Effect &effect) -> Float {
  return CORE::OSCILLATOR::saw(effect.carrier);
}

auto square(const RINGMOD::Effect &effect) -> Float {
  return CORE::OSCILLATOR::pulse(effect.carrier);
}

constexpr Carrier CARRIERS[] = {sine, triangle, saw, square};
static_assert(sizeof(CARRIERS) / sizeof(CARRIERS[0]) == RINGMOD::WAVES);

}  // namespace

void SOUND::RINGMOD::settle(Effect &effect) {
  CORE::OSCILLATOR::settle(
    effect.carrier, effect.rows[FREQUENCY], CORE::OSCILLATOR::SQUARE,
    effect.rate);
  const Whole wave = Whole(effect.rows[WAVE]);
  effect.wave = wave < WAVES ? wave : SINE;
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}

auto SOUND::RINGMOD::tick(Effect &effect) -> Float {
  const Float value = ::CARRIERS[effect.wave](effect);
  effect.carrier.phase += effect.carrier.step;
  return value;
}

void SOUND::RINGMOD::apply(Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
