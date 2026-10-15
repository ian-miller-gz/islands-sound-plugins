// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1;
constexpr Float DRIVING = 8;
constexpr Float LEAN = 0.2f;
constexpr Float SINK = 1;
constexpr Float ATTACK = 0.01f;
constexpr Float RELEASE = 0.3f;
constexpr Float BLOCKED = 20;

struct Curve {
  Float gain;
  Float bias;
  Float rest;
};

auto bend(const Curve &curve, Float in) -> Float {
  return CORE::SHAPER::soft(in * curve.gain + curve.bias) - curve.rest;
}

}  // namespace

void SOUND::PLUGINS::DRAWBAR::build(Preamp &preamp, Whole rate) {
  preamp.follower.attack = ::ATTACK;
  preamp.follower.release = ::RELEASE;
  preamp.follower.kind = CORE::DYNAMICS::PEAK;
  CORE::DYNAMICS::settle(preamp.follower, rate);
  CORE::SHAPER::settle(preamp.blocker, ::BLOCKED, rate);
}

auto SOUND::PLUGINS::DRAWBAR::tick(Preamp &preamp, const Float *rows, Float in)
  -> Float {
  const Float driven = rows[DRIVE];
  if (driven <= 0) return in;
  const Float gain = ::FULL + ::DRIVING * driven;
  const Float level = CORE::DYNAMICS::tick(preamp.follower, in);
  const Float sag = rows[SAG] * ::SINK * CORE::SHAPER::soft(level);
  const Float bias = ::LEAN + sag;
  const ::Curve curve = {gain, bias, CORE::SHAPER::soft(bias)};
  const auto shape = [&curve](Float sample) { return ::bend(curve, sample); };
  const Float out = CORE::SHAPER::tick(preamp.oversampler, in, shape);
  return CORE::SHAPER::tick(preamp.blocker, out) / CORE::SHAPER::soft(gain);
}
