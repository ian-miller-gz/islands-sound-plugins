// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float PI = 3.14159265f;
constexpr Float SILENT = 0.001f;
constexpr Float SHORTEST = 1.0f;
constexpr Float NYQUIST = 0.45f;

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::struck(Float velocity) -> Float {
  return velocity < 0 ? 0 : velocity > ONE ? ONE : velocity;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::turn(Float hertz, Whole rate) -> Float {
  const Float top = Float(rate) * NYQUIST;
  const Float held = hertz < 0 ? 0 : hertz > top ? top : hertz;
  return TWO * std::sin(PI * held / Float(rate));
}

auto SOUND::PLUGINS::CORE::PERCUSSION::fall(Float seconds, Whole rate)
  -> Float {
  const Float frames = seconds * Float(rate);
  if (frames <= SHORTEST) return 0;
  return std::exp(std::log(SILENT) / frames);
}

void SOUND::PLUGINS::CORE::PERCUSSION::shape(
  ENVELOPE::Envelope &envelope, Float seconds, Whole rate) {
  envelope = ENVELOPE::AD::create(0, seconds);
  envelope.curve = ENVELOPE::EXPONENTIAL;
  ENVELOPE::shape(envelope, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Resonator &resonator, Float hertz, Float seconds, Whole rate) {
  resonator.turn = turn(hertz, rate);
  resonator.damp = fall(seconds, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Resonator &resonator, Float level) {
  resonator.cosine += level;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Resonator &resonator) -> Float {
  resonator.sine += resonator.turn * resonator.cosine;
  resonator.cosine -= resonator.turn * resonator.sine;
  resonator.sine *= resonator.damp;
  resonator.cosine *= resonator.damp;
  return resonator.sine;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::keyed(
  const Key *keys, Whole count, Whole pitch, Whole fallback) -> Whole {
  for (Whole at = 0; at < count; ++at)
    if (keys[at].pitch == pitch) return keys[at].drum;
  return fallback;
}
