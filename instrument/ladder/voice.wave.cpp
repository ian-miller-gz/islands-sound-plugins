// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::OSCILLATOR::Oscillator;

constexpr Float HALF = 0.5f;

namespace WIDTH {
constexpr Float SQUARE = CORE::OSCILLATOR::SQUARE;
constexpr Float WIDE = 0.3f;
constexpr Float NARROW = 0.12f;
}  // namespace WIDTH

auto triangle(const Oscillator &oscillator) -> Float {
  return CORE::OSCILLATOR::triangle(oscillator);
}

auto sharktooth(const Oscillator &oscillator) -> Float {
  return (CORE::OSCILLATOR::triangle(oscillator) +
          CORE::OSCILLATOR::saw(oscillator)) *
         HALF;
}

auto reverse(const Oscillator &oscillator) -> Float {
  return -CORE::OSCILLATOR::saw(oscillator);
}

auto saw(const Oscillator &oscillator) -> Float {
  return CORE::OSCILLATOR::saw(oscillator);
}

auto pulse(const Oscillator &oscillator) -> Float {
  return CORE::OSCILLATOR::pulse(oscillator);
}

using Form = auto (*)(const Oscillator &) -> Float;

constexpr Form FORMS[LADDER::SHAPES] = {triangle, sharktooth, reverse, saw,
                                        pulse,    pulse,      pulse};
constexpr Float WIDTHS[LADDER::SHAPES] = {
  WIDTH::SQUARE, WIDTH::SQUARE, WIDTH::SQUARE, WIDTH::SQUARE,
  WIDTH::SQUARE, WIDTH::WIDE,   WIDTH::NARROW};

}  // namespace

auto SOUND::PLUGINS::LADDER::sing(Source &source, Float pitch, Whole rate)
  -> Float {
  Oscillator &oscillator = source.oscillator;
  const Float hertz = CORE::PHASE::hertz(pitch + source.offset);
  CORE::OSCILLATOR::settle(oscillator, hertz, ::WIDTHS[source.shape], rate);
  const Float value = ::FORMS[source.shape](oscillator);
  oscillator.phase += oscillator.step;
  return value;
}
