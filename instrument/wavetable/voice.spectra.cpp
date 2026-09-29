// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <numbers>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float UNIT = 1;
constexpr Float PI = std::numbers::pi_v<Float>;
constexpr Float SINES = 8;
constexpr Float SQUARE = 0.5f;
constexpr Float NARROWING = 0.45f;
constexpr Whole PARITY = 2;
constexpr Whole QUARTER = 4;
constexpr Whole FALLING = 3;
constexpr Whole DRAWBARS[] = {1, 2, 3, 4, 6, 8, 10, 12, 16};

auto fade(Float place, Float harmonic) -> Float {
  return std::pow(place, std::log2(harmonic));
}

auto odd(Float harmonic) -> Flag { return Whole(harmonic) % PARITY != 0; }

}  // namespace

auto SOUND::WAVETABLE::SPECTRUM::sine(Float place, Float harmonic) -> Float {
  const Float centre = UNIT + place * (SINES - UNIT);
  return std::max(0.0f, UNIT - std::fabs(harmonic - centre));
}

auto SOUND::WAVETABLE::SPECTRUM::harmonic(Float place, Float harmonic)
  -> Float {
  const Float edge = UNIT + place * Float(PARTIALS - 1);
  return std::clamp(edge - harmonic + UNIT, 0.0f, UNIT) / harmonic;
}

auto SOUND::WAVETABLE::SPECTRUM::saw(Float place, Float harmonic) -> Float {
  return ::fade(place, harmonic) / harmonic;
}

auto SOUND::WAVETABLE::SPECTRUM::square(Float place, Float harmonic) -> Float {
  return ::odd(harmonic) ? saw(place, harmonic) : 0;
}

auto SOUND::WAVETABLE::SPECTRUM::hollow(Float place, Float harmonic) -> Float {
  return harmonic <= UNIT || !::odd(harmonic) ? saw(place, harmonic) : 0;
}

auto SOUND::WAVETABLE::SPECTRUM::triangle(Float place, Float harmonic)
  -> Float {
  if (!::odd(harmonic)) return 0;
  const Float sign = Whole(harmonic) % QUARTER == FALLING ? -UNIT : UNIT;
  return (UNIT - place) * sign / (harmonic * harmonic) + place / harmonic;
}

auto SOUND::WAVETABLE::SPECTRUM::pulse(Float place, Float harmonic) -> Float {
  const Float width = SQUARE - NARROWING * place;
  return std::sin(PI * harmonic * width) / harmonic;
}

auto SOUND::WAVETABLE::SPECTRUM::organ(Float place, Float harmonic) -> Float {
  const Whole at = Whole(harmonic);
  const Flag drawn = std::ranges::find(::DRAWBARS, at) != std::end(::DRAWBARS);
  return drawn ? ::fade(place, harmonic) : 0;
}

auto SOUND::WAVETABLE::SPECTRUM::octaves(Float place, Float harmonic) -> Float {
  return std::has_single_bit(Whole(harmonic)) ? ::fade(place, harmonic) : 0;
}
