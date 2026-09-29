// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::DYNAMICS::decibels;

constexpr Float SPAN = 6.0f;
constexpr Float ONE = 1.0f;

auto portion(Float difference) -> Float {
  if (difference <= 0) return 0;
  return difference >= ::SPAN ? ::ONE : difference / ::SPAN;
}

auto heard(CORE::DYNAMICS::Detector &detector, Float in) -> Float {
  return decibels(CORE::DYNAMICS::tick(detector, in));
}

}  // namespace

auto SOUND::PLUGINS::BUS::shape(Bus &bus, Float in) -> Float {
  const Float fast = ::heard(bus.detectors.fast, in);
  const Float slow = ::heard(bus.detectors.slow, in);
  const Float tail = ::heard(bus.detectors.tail, in);
  const Float onset = ::portion(fast - slow);
  const Float ring = ::portion(tail - fast);
  return CORE::DYNAMICS::gain(
    bus.rows[ATTACK] * onset + bus.rows[SUSTAIN] * ring);
}

auto SOUND::PLUGINS::BUS::squeeze(Bus &bus, Float in) -> Float {
  const Float level = ::heard(bus.detectors.glue, in);
  return CORE::DYNAMICS::gain(CORE::DYNAMICS::reduce(bus.computer, level));
}
