// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdint>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float BASE = 4;
constexpr Float SYNCS = 7;
constexpr Float MIDDLE = 0.5f;
constexpr Float NARROWING = 0.45f;
constexpr Float SQUEEZE = 0.9f;
constexpr Float FULL = 1;
constexpr Whole FIRST = 0;

auto bend(Float phase, Float slope) -> Float { return std::pow(phase, slope); }

auto bent(Float amount) -> Float { return std::pow(BASE, amount); }

auto sync(Float phase, Float slope) -> Float {
  const Float turns = phase * slope;
  return turns - std::floor(turns);
}

auto synced(Float amount) -> Float { return FULL + SYNCS * amount; }

auto knee(Float phase, Float slope) -> Float {
  const Float edge = MIDDLE / slope;
  if (phase < edge) return MIDDLE * phase / edge;
  return MIDDLE + MIDDLE * (phase - edge) / (FULL - edge);
}

auto kneed(Float amount) -> Float {
  return MIDDLE / (MIDDLE - NARROWING * amount);
}

auto pulse(Float phase, Float slope) -> Float {
  return std::fmin(phase * slope, FULL);
}

auto pulsed(Float amount) -> Float { return FULL / (FULL - SQUEEZE * amount); }

using Shape = auto (*)(Float phase, Float slope) -> Float;
using Slope = auto (*)(Float amount) -> Float;

struct Warp {
  Shape shape;
  Slope slope;
};

constexpr Warp WARPS[] = {
  {bend, bent}, {sync, synced}, {knee, kneed}, {pulse, pulsed}};
static_assert(std::size(WARPS) == std::size(WAVETABLE::WARPS));

auto chosen(Whole kind) -> const Warp & {
  return WARPS[kind < std::size(WARPS) ? kind : FIRST];
}

}  // namespace

auto SOUND::PLUGINS::WAVETABLE::slope(Whole kind, Float amount) -> Float {
  return ::chosen(kind).slope(amount);
}

auto SOUND::PLUGINS::WAVETABLE::warp(
  const Synth &synth, CORE::PHASE::Wheel phase) -> CORE::PHASE::Wheel {
  const Float place = CORE::PHASE::fraction(phase);
  const Float bent = ::chosen(synth.warp).shape(place, synth.slope);
  return CORE::PHASE::Wheel(std::uint64_t(bent * CORE::PHASE::TURN));
}
