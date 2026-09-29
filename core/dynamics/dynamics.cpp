// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "dynamics.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float TEN = 10.0f;
constexpr Float TWENTY = 20.0f;
constexpr Float HALF = 0.5f;
constexpr Float DOUBLE = 2.0f;

constexpr Float QUIET = 0.000001f;

auto factor(const DYNAMICS::Computer &computer) -> Float {
  const Float ratio =
    computer.ratio < DYNAMICS::UNITY ? DYNAMICS::UNITY : computer.ratio;
  return computer.side == DYNAMICS::BELOW
           ? DYNAMICS::UNITY - ratio
           : DYNAMICS::UNITY / ratio - DYNAMICS::UNITY;
}

}  // namespace

auto SOUND::CORE::DYNAMICS::gain(Float decibels) -> Float {
  if (decibels <= SILENCE) return 0;
  return std::pow(::TEN, decibels / ::TWENTY);
}

auto SOUND::CORE::DYNAMICS::decibels(Float gain) -> Float {
  const Float size = gain < 0 ? -gain : gain;
  if (size <= ::QUIET) return SILENCE;
  return ::TWENTY * std::log10(size);
}

void SOUND::CORE::DYNAMICS::settle(Computer &computer) {
  computer.factor = ::factor(computer);
}

auto SOUND::CORE::DYNAMICS::reduce(const Computer &computer, Float decibels)
  -> Float {
  const Float over = decibels - computer.threshold;
  const Float depth = computer.side == BELOW ? -over : over;
  const Float half = computer.knee * ::HALF;
  if (depth <= -half) return 0;
  if (depth >= half) return computer.factor * depth;
  const Float into = depth + half;
  return computer.factor * into * into / (::DOUBLE * computer.knee);
}
