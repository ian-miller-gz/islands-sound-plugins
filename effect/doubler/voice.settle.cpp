// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float GLIDE = 0.5f;
constexpr Float FARTHEST = 1.3f;
constexpr Float BOTH = 2.0f;
constexpr Whole PAIRED = 2;
constexpr CORE::MODULATOR::Seed SCATTER = 0x2545F491u;

auto frames(const DOUBLER::Doubler &doubler, Float milliseconds) -> Float {
  return milliseconds * Float(doubler.rate) / DOUBLER::MILLI;
}

void seat(
  DOUBLER::Copy &copy, const DOUBLER::Seat &place, Whole index, Whole rate) {
  copy.wobble.wave = CORE::MODULATOR::RANDOM;
  copy.wobble.hertz = place.hertz;
  copy.wobble.slew = ::GLIDE / place.hertz;
  copy.wobble.seed =
    CORE::MODULATOR::SEED ^ CORE::MODULATOR::Seed((index + 1) * ::SCATTER);
  CORE::MODULATOR::settle(copy.wobble, rate);
  CORE::MODULATOR::reset(copy.wobble);
}

}  // namespace

void SOUND::PLUGINS::DOUBLER::build(Doubler &doubler) {
  const Float longest =
    ::frames(doubler, LONGEST * ::FARTHEST + SWING * ::BOTH);
  CORE::LINE::build(doubler.line, Whole(longest) + 1);
  for (Whole index = 0; index < COPIES; ++index) {
    Copy &copy = doubler.copies[index];
    CORE::PITCH::build(copy.grains, WINDOW, doubler.rate);
    ::seat(copy, SEATS[index], index, doubler.rate);
  }
}

void SOUND::PLUGINS::DOUBLER::settle(Doubler &doubler) {
  doubler.count = doubler.rows[VOICES] >= ::HALF ? COPIES : ::PAIRED;
  doubler.scale = ::UNITY / std::sqrt(Float(doubler.count));
  doubler.swing = ::frames(doubler, doubler.rows[WOBBLE] * SWING);
  const Float base = ::frames(doubler, doubler.rows[DELAY]);
  for (Whole index = 0; index < COPIES; ++index) {
    Copy &copy = doubler.copies[index];
    const Seat &place = SEATS[index];
    copy.delay = base * place.delay;
    const Float cents = place.detune * doubler.rows[DETUNE];
    CORE::PITCH::settle(copy.grains, cents, WINDOW, doubler.rate);
    const Float pan = place.pan * doubler.rows[WIDTH];
    copy.left = pan > 0 ? ::UNITY - pan : ::UNITY;
    copy.right = pan < 0 ? ::UNITY + pan : ::UNITY;
  }
}

void SOUND::PLUGINS::DOUBLER::apply(
  Doubler &doubler, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  doubler.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(doubler);
}
