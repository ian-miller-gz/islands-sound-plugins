// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float TAU = 2.0f * std::numbers::pi_v<Float>;

auto reach(Whole rate) -> Whole {
  const Float swing = LOFI::excursion(LOFI::DEEPEST);
  const Float seconds = LOFI::MARGIN + LOFI::DOUBLE * swing;
  return Whole(std::ceil(seconds * Float(rate)));
}

}  // namespace

auto SOUND::PLUGINS::LOFI::excursion(Float cents) -> Float {
  return (CORE::PHASE::ratio(cents) - UNITY) / (::TAU * WOBBLE);
}

void SOUND::PLUGINS::LOFI::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  CORE::NOISE::Register seed = CORE::NOISE::SEED;
  for (Strip &strip : effect.strips) {
    CORE::LINE::build(strip.line, ::reach(effect.rate));
    CORE::NOISE::seed(strip.white, seed);
    seed += CORE::NOISE::SEED;
  }
  effect.lfo.wave = CORE::MODULATOR::SINE;
  effect.lfo.hertz = WOBBLE;
  CORE::MODULATOR::settle(effect.lfo, effect.rate);
  CORE::MODULATOR::reset(effect.lfo);
}

void SOUND::PLUGINS::LOFI::settle(Effect &effect) {
  const Float swing = excursion(effect.rows[WOW]);
  const Float band = effect.rows[BAND];
  for (Strip &strip : effect.strips) {
    CORE::LINE::settle(strip.sweep, MARGIN + swing, swing, GLIDE, effect.rate);
    CORE::FILTER::settle(
      strip.before, CORE::FILTER::LOW, band, CORE::FILTER::FLAT, 0,
      effect.rate);
    CORE::FILTER::settle(
      strip.after, CORE::FILTER::LOW, band, CORE::FILTER::FLAT, 0, effect.rate);
    CORE::SHAPER::settle(
      strip.crusher, effect.rows[BITS], effect.rows[RATE], effect.rate);
  }
  effect.floor = std::pow(TEN, effect.rows[NOISE] / DECIBELS);
}

void SOUND::PLUGINS::LOFI::apply(
  Effect &effect, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  effect.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(effect);
}
