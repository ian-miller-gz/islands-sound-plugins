// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "shaper.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float HALF = 0.5f;
constexpr Float PERIOD = 4.0f;

}  // namespace

auto SOUND::PLUGINS::CORE::SHAPER::tick(
  Integral &integral, Float in, Curve curve, Curve area) -> Float {
  const Float next = area(in);
  const Float step = in - integral.last;
  const Float out = std::fabs(step) < NEAR ? curve(HALF * (in + integral.last))
                                           : (next - integral.area) / step;
  integral.last = in;
  integral.area = next;
  return out;
}

auto SOUND::PLUGINS::CORE::SHAPER::AREA::hard(Float in) -> Float {
  const Float size = std::fabs(in);
  return size < ONE ? HALF * in * in : size - HALF;
}

auto SOUND::PLUGINS::CORE::SHAPER::AREA::fold(Float in) -> Float {
  const Float shifted = in - ONE;
  const Float wrapped = shifted - PERIOD * std::floor(shifted / PERIOD);
  const Float centred = wrapped - TWO;
  return centred * (HALF * std::fabs(centred) - ONE) + HALF;
}
