// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "../../core/filter/filter.hpp"
#include "../../core/noise/noise.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Whole RATE = 48000;
constexpr Whole HALF = RATE / 2;
constexpr Float TURN = 6.28318531f;
constexpr Float CUTOFF = 1000;
constexpr Float SLOW = 200;
constexpr Float MIDDLE = 0.5f;
constexpr Float RINGING = 0.95f;
constexpr Float FULL = 1;
constexpr Float LOUD = 4;
constexpr Float FAINT = 0.001f;

void same() {
  CORE::NOISE::White white;
  CORE::FILTER::Variable five, six;
  Whole differing = 0;
  for (Whole kind = 0; kind <= CORE::FILTER::NOTCH; ++kind) {
    CORE::FILTER::settle(five, kind, CUTOFF, MIDDLE, RATE);
    CORE::FILTER::settle(six, kind, CUTOFF, MIDDLE, 0, RATE);
    for (Whole at = 0; at < RATE; ++at) {
      const Float in = CORE::NOISE::tick(white);
      const Float first = CORE::FILTER::tick(five, in);
      const Float second = CORE::FILTER::tick(six, in);
      differing += std::bit_cast<Bits>(first) != std::bit_cast<Bits>(second);
    }
  }
  std::printf("same %zu\n", differing);
}

auto peak(Float saturation, Float level, Float hertz, Whole from) -> Float {
  CORE::FILTER::Variable filter;
  CORE::FILTER::settle(
    filter, CORE::FILTER::LOW, CUTOFF, RINGING, saturation, RATE);
  Float most = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Float in = level * std::sin(TURN * hertz * Float(at) / Float(RATE));
    const Float out = CORE::FILTER::tick(filter, in);
    if (at >= from) most = std::fmax(most, std::fabs(out));
  }
  return most;
}

void print(STRING::Hot word, Float level, Float hertz, Whole from) {
  const Float plain = ::peak(0, level, hertz, from);
  const Float saturated = ::peak(FULL, level, hertz, from);
  std::printf("%s %.9g %.9g\n", word, double(plain), double(saturated));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::FILTER {

void variable() {
  ::same();
  ::print("loud", LOUD, CUTOFF, 0);
  ::print("faint", FAINT, SLOW, HALF);
}

}  // namespace SOUND::PLUGINS::HARNESS::FILTER
