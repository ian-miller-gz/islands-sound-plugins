// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

#include "../modulator/modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TAU = 6.2831853f;
constexpr Float DEPTH = 2.0f;
constexpr Float SETTLING = 0.25f;
constexpr Float QUICK = 0.15f;
constexpr Float DARK = 0.1f;
constexpr Float NOISY = 2.0f;
constexpr Float WIDTH = 0.001f;
constexpr Float DRIVE = 1.2f;

auto swept(Float hertz, Whole rate) -> Float {
  return MODULATOR::pole(ONE / (TAU * hertz), rate);
}

auto spike(PERCUSSION::Pad &pad) -> Float {
  if (pad.left == 0) return 0;
  --pad.left;
  return pad.click;
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Pad &pad, const Voicing &voicing, Whole rate) {
  const HYBRID::Bend bend = {
    .depth = voicing.bend * ::DEPTH,
    .fast = voicing.decay * ::QUICK,
    .knee = voicing.bend * ::SETTLING,
    .slow = voicing.decay};
  HYBRID::settle(pad.tone, bend, rate);
  pad.pace = HYBRID::pace(voicing.tune, rate);
  pad.decay = ENVELOPE::AD::create(0, voicing.decay);
  ENVELOPE::shape(pad.decay, rate);
  pad.top = ::swept(voicing.tone, rate);
  pad.floor = ::swept(voicing.tone * ::DARK, rate);
  pad.noise = voicing.noise;
  pad.click = voicing.click;
  pad.width = Whole(::WIDTH * Float(rate));
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Pad &pad, Float velocity) {
  pad.velocity = struck(velocity);
  HYBRID::strike(pad.tone, pad.pace);
  ENVELOPE::strike(pad.gate, pad.decay, ONE);
  pad.left = pad.width;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Pad &pad) -> Float {
  if (!ENVELOPE::sounding(pad.gate)) return 0;
  const Float level = ENVELOPE::tick(pad.gate, pad.decay);
  const Float pole = pad.floor + (pad.top - pad.floor) * level;
  pad.low += (NOISE::tick(pad.white) - pad.low) * pole;
  const Float tone = HYBRID::tick(pad.tone) * (::ONE - pad.noise);
  const Float hiss = pad.low * pad.noise * ::NOISY;
  const Float body = (tone + hiss) * level + ::spike(pad);
  return SHAPER::soft(body * pad.velocity * ::DRIVE);
}
