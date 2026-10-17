// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float RESIDUE = 0.01f;

auto ease(Float step) -> Float {
  if (step >= MODULATOR::FULL) return MODULATOR::FULL;
  return -std::expm1(step * std::log(RESIDUE));
}

auto even(Float distance, Float step) -> Float {
  return distance < step ? distance : step;
}

auto inverse(Float distance, Float step) -> Float {
  const Float left = distance * distance - step;
  return left > 0 ? distance - std::sqrt(left) : distance;
}

auto moved(Float distance, Float step, Float pole, Float shape) -> Float {
  const Float linear = even(distance, step);
  const Float bent = shape > 0 ? distance * pole : inverse(distance, step);
  const Float size = std::fabs(shape);
  const Float weight = size < MODULATOR::FULL ? size : MODULATOR::FULL;
  return linear + (bent - linear) * weight;
}

}  // namespace

void SOUND::PLUGINS::CORE::MODULATOR::settle(Slew &slew, Whole rate) {
  slew.up = delta(slew.rise, rate);
  slew.down = delta(slew.fall, rate);
  slew.lift = ::ease(slew.up);
  slew.drop = ::ease(slew.down);
}

void SOUND::PLUGINS::CORE::MODULATOR::jump(Slew &slew, Float value) {
  slew.value = value;
}

auto SOUND::PLUGINS::CORE::MODULATOR::tick(Slew &slew, Float in) -> Float {
  const Float change = in - slew.value;
  slew.value += change > 0
                  ? ::moved(change, slew.up, slew.lift, slew.shape)
                  : -::moved(-change, slew.down, slew.drop, slew.shape);
  return slew.value;
}
