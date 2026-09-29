// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void high(CORE::FILTER::Biquad &biquad, Float cutoff, Whole rate) {
  CORE::FILTER::settle(
    biquad, CORE::FILTER::HIGH, cutoff, CORE::FILTER::FLAT, 0, rate);
}

}  // namespace

void SOUND::EXCITER::build(Effect &effect) {
  effect.strips.resize(effect.channels);
}

void SOUND::EXCITER::settle(Effect &effect) {
  for (Strip &strip : effect.strips) {
    ::high(strip.split, effect.rows[FREQUENCY], effect.rate);
    ::high(strip.clean, effect.rows[FREQUENCY], effect.rate);
  }
  effect.drive = std::pow(TEN, effect.rows[DRIVE] / DECIBELS);
  effect.trim = UNITY / effect.drive;
  effect.mix = effect.rows[MIX];
}

void SOUND::EXCITER::apply(Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
