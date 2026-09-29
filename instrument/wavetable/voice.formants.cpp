// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numbers>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNIT = 1;
constexpr Float PI = std::numbers::pi_v<Float>;
constexpr Float REACH = 24;
constexpr Float BROAD = 2;
constexpr Float FLOOR = 0.1f;
constexpr Float SECOND = 0.5f;
constexpr Float BASE = 110;
constexpr Float SPREAD = 6;
constexpr Float HOLLOW = 2;
constexpr Float SYNCS = 8;
constexpr Float TINY = 0.0001f;
constexpr Float TILT = 2;
constexpr Float HALF = 0.5f;
constexpr Float PEAKING = 4;
constexpr CORE::MODULATOR::Seed MIXING = 0x2545F491u;
constexpr Whole PRIMES[] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

struct Vowel {
  Float first;
  Float second;
};

constexpr Vowel VOWELS[] = {
  {800, 1150}, {350, 2000}, {270, 2140}, {450, 800}, {325, 700}};

auto bump(Float harmonic, Float centre) -> Float {
  const Float distance = harmonic - centre;
  return std::exp(-distance * distance / BROAD);
}

auto sinc(Float x) -> Float {
  return std::fabs(x) < TINY ? UNIT : std::sin(PI * x) / (PI * x);
}

}  // namespace

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::formant(Float place, Float harmonic)
  -> Float {
  const Float centre = UNIT + place * (REACH - UNIT);
  return FLOOR / harmonic + ::bump(harmonic, centre);
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::vowel(Float place, Float harmonic)
  -> Float {
  const Float top = Float(std::size(::VOWELS) - 1);
  const Float held = place * top;
  const Whole low = std::min(Whole(held), Whole(top) - 1);
  const Float part = held - Float(low);
  const Vowel &from = ::VOWELS[low];
  const Vowel &to = ::VOWELS[low + 1];
  const Float first = from.first + (to.first - from.first) * part;
  const Float second = from.second + (to.second - from.second) * part;
  return FLOOR / harmonic + ::bump(harmonic, first / BASE) +
         SECOND * ::bump(harmonic, second / BASE);
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::comb(Float place, Float harmonic)
  -> Float {
  const Float period = HOLLOW + place * SPREAD;
  return std::fabs(std::cos(PI * harmonic / period)) / harmonic;
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::sync(Float place, Float harmonic)
  -> Float {
  return ::sinc(harmonic - (UNIT + place * (SYNCS - UNIT)));
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::prime(Float place, Float harmonic)
  -> Float {
  const Whole at = Whole(harmonic);
  const Flag found = std::ranges::find(::PRIMES, at) != std::end(::PRIMES);
  return found ? std::pow(harmonic, -(UNIT - place) * TILT) : 0;
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::scatter(Float place, Float harmonic)
  -> Float {
  CORE::MODULATOR::Seed seed =
    CORE::MODULATOR::SEED * (Whole(harmonic) * MIXING);
  const Float from = CORE::MODULATOR::draw(seed);
  const Float to = CORE::MODULATOR::draw(seed);
  return (from + (to - from) * place) / std::pow(harmonic, HALF);
}

auto SOUND::PLUGINS::WAVETABLE::SPECTRUM::resonant(Float place, Float harmonic)
  -> Float {
  const Float centre = UNIT + place * (REACH - UNIT);
  const Float low = harmonic <= centre ? UNIT : std::exp(centre - harmonic);
  return (low + PEAKING * ::bump(harmonic, centre)) / harmonic;
}
