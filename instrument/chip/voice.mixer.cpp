// SPDX-License-Identifier: AGPL-3.0-or-later
#include <array>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole BIT = 1;
constexpr Whole TOP = 14;
constexpr Whole SQUARES = CHIP::PULSES * Whole(CHIP::LOUDEST) + 1;
constexpr Whole WEIGHT = 3;
constexpr Whole HISS = 2;
constexpr Whole OTHERS = (WEIGHT + HISS) * Whole(CHIP::LOUDEST) + 1;
constexpr Float BASE = 100;

struct Curve {
  Float scale;
  Float span;
};

constexpr Curve SQUARE = {95.52f, 8128};
constexpr Curve OTHER = {163.67f, 24329};

template <Whole SIZE>
constexpr auto table(const Curve &curve) -> std::array<Float, SIZE> {
  std::array<Float, SIZE> values{};
  for (Whole at = 1; at < SIZE; ++at)
    values[at] = curve.scale / (curve.span / Float(at) + BASE);
  return values;
}

constexpr auto PULSED = table<SQUARES>(SQUARE);
constexpr auto SHARED = table<OTHERS>(OTHER);

}  // namespace

auto SOUND::PLUGINS::CHIP::shift(Lfsr &noise) -> Whole {
  const Whole feedback = (noise.bits ^ (noise.bits >> noise.tap)) & ::BIT;
  noise.bits = (noise.bits >> 1) | (feedback << ::TOP);
  return noise.bits & ::BIT;
}

auto SOUND::PLUGINS::CHIP::mixed(Whole pulses, Whole triangle, Whole noise)
  -> Float {
  const Whole others = ::WEIGHT * triangle + ::HISS * noise;
  const Float square = ::PULSED[pulses < ::SQUARES ? pulses : ::SQUARES - 1];
  return square + ::SHARED[others < ::OTHERS ? others : ::OTHERS - 1];
}
