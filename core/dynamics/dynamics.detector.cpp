// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "dynamics.hpp"

#include "../modulator/modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float TAU = 6.2831853f;

auto peak(Float in) -> Float { return in < 0 ? -in : in; }

auto square(Float in) -> Float { return in * in; }

auto level(Float held) -> Float { return held; }

auto root(Float held) -> Float { return std::sqrt(held); }

using Measure = auto (*)(Float) -> Float;

constexpr Measure MEASURES[DYNAMICS::KINDS] = {peak, square};
constexpr Measure READINGS[DYNAMICS::KINDS] = {level, root};

auto kind(const DYNAMICS::Detector &detector) -> Whole {
  return detector.kind < DYNAMICS::KINDS ? detector.kind : DYNAMICS::PEAK;
}

}  // namespace

void SOUND::PLUGINS::CORE::DYNAMICS::settle(Detector &detector, Whole rate) {
  detector.rise = MODULATOR::pole(detector.attack, rate);
  detector.fall = MODULATOR::pole(detector.release, rate);
}

auto SOUND::PLUGINS::CORE::DYNAMICS::tick(Detector &detector, Float in)
  -> Float {
  const Whole measured = ::kind(detector);
  const Float heard = ::MEASURES[measured](in);
  const Float pole = heard > detector.level ? detector.rise : detector.fall;
  detector.level += (heard - detector.level) * pole;
  return ::READINGS[measured](detector.level);
}

auto SOUND::PLUGINS::CORE::DYNAMICS::RING::create(Whole capacity) -> Ring {
  Ring ring;
  ring.samples.assign(capacity + 1, 0);
  return ring;
}

void SOUND::PLUGINS::CORE::DYNAMICS::delay(Ring &ring, Whole length) {
  const Whole most = ring.samples.empty() ? 0 : ring.samples.size() - 1;
  ring.length = length > most ? most : length;
}

auto SOUND::PLUGINS::CORE::DYNAMICS::tick(Ring &ring, Float in) -> Float {
  if (ring.samples.empty()) return in;
  const Whole size = ring.samples.size();
  ring.samples[ring.at] = in;
  const Whole from = (ring.at + size - ring.length) % size;
  ring.at = (ring.at + 1) % size;
  return ring.samples[from];
}

void SOUND::PLUGINS::CORE::DYNAMICS::settle(Highpass &highpass, Whole rate) {
  const Float turn = rate == 0 ? 0 : TAU * highpass.hertz / Float(rate);
  highpass.pole = turn <= 0 ? 0 : UNITY - std::exp(-turn);
}

auto SOUND::PLUGINS::CORE::DYNAMICS::tick(Highpass &highpass, Float in)
  -> Float {
  highpass.low += (in - highpass.low) * highpass.pole;
  return in - highpass.low;
}
