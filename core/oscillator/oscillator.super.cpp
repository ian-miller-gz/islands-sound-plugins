// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float CURVE[] = {
  10028.7312891634f,   -50818.8652045924f, 111363.4808729368f,
  -138150.6761080548f, 106649.6679158292f, -53046.9642751875f,
  17019.9518580080f,   -3425.0836591318f,  404.2703938388f,
  -24.1878824391f,     0.6717417634f,      0.0030115596f};

constexpr Float OFFSETS[] = {-0.11002313f, -0.06288439f, -0.01952356f, 0,
                             0.01991221f,  0.06216538f,  0.10745242f};
static_assert(sizeof(OFFSETS) / sizeof(OFFSETS[0]) == OSCILLATOR::SAWS);

constexpr Float MIDDLE[] = {-0.55366f, 0.99785f};
constexpr Float SIDES[] = {-0.73764f, 1.2841f, 0.044372f};

constexpr OSCILLATOR::Wheel GOLDEN = 0x9E3779B9u;
constexpr Float UNIT = 1.0f;
constexpr Float SQUARE = 0.5f;

template <Whole COUNT>
auto horner(const Float (&terms)[COUNT], Float x) -> Float {
  Float sum = 0;
  for (const Float term : terms) sum = sum * x + term;
  return sum;
}

auto unit(Float value) -> Float {
  return value < 0 ? 0 : value > UNIT ? UNIT : value;
}

}  // namespace

auto SOUND::CORE::OSCILLATOR::spread(Float detune) -> Float {
  return ::horner(::CURVE, ::unit(detune));
}

auto SOUND::CORE::OSCILLATOR::centre(Float mix) -> Float {
  return ::horner(::MIDDLE, ::unit(mix));
}

auto SOUND::CORE::OSCILLATOR::side(Float mix) -> Float {
  return ::horner(::SIDES, ::unit(mix));
}

void SOUND::CORE::OSCILLATOR::settle(
  Super &super, Float hertz, Float detune, Float mix, Whole rate) {
  const Float amount = spread(detune);
  for (Whole saw = 0; saw < SAWS; ++saw) {
    const Float ratio = UNIT + amount * ::OFFSETS[saw];
    settle(super.saws[saw], hertz * ratio, ::SQUARE, rate);
  }
  super.centre = centre(mix);
  super.side = side(mix);
}

void SOUND::CORE::OSCILLATOR::scatter(Super &super, Wheel seed) {
  Wheel phase = seed;
  for (Oscillator &saw : super.saws) {
    phase += ::GOLDEN;
    reset(saw, phase);
  }
}

auto SOUND::CORE::OSCILLATOR::tick(Super &super) -> Float {
  Float sides = 0;
  for (Whole saw = 0; saw < SAWS; ++saw)
    if (saw != MIDDLE) sides += tick(super.saws[saw], SAW);
  return tick(super.saws[MIDDLE], SAW) * super.centre + sides * super.side;
}
