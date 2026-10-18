// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;
using PERCUSSION::HYBRID::STROKE::STROKES;

constexpr Float LENGTH = 0.6f;
constexpr PERCUSSION::HYBRID::Colour DARK = {.band = 5000.0f, .edge = 3000.0f};
constexpr Float SPANS[STROKES] = {1.0f, 0.5f};
constexpr Float GAIN = 1.5f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::build(
  Cymbal &cymbal, Whole rate) {
  build(cymbal.loop, ::LENGTH, ::DARK, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(
  Cymbal &cymbal, Float tune, Float decay, Whole rate) {
  settle(cymbal.loop, tune);
  for (Whole stroke = 0; stroke < STROKES; ++stroke)
    shape(cymbal.envelopes[stroke], decay * ::SPANS[stroke], rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(
  Cymbal &cymbal, Whole stroke, Float velocity) {
  cymbal.stroke = stroke < STROKES ? stroke : STROKE::CRASH;
  cymbal.velocity = struck(velocity);
  strike(cymbal.loop);
  ENVELOPE::strike(
    cymbal.gate, cymbal.envelopes[cymbal.stroke], cymbal.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Cymbal &cymbal) -> Float {
  if (!sounding(cymbal)) return 0;
  const Float sample = tick(cymbal.loop);
  const Float level =
    ENVELOPE::tick(cymbal.gate, cymbal.envelopes[cymbal.stroke]);
  return SHAPER::soft(sample * level * cymbal.velocity * ::GAIN);
}
