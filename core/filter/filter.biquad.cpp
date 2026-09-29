// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float TURN = 6.28318531f;
constexpr Float TEN = 10.0f;
constexpr Float DECIBELS = 40.0f;
constexpr Float NARROWEST = 0.05f;

struct Angle {
  Float cosine;
  Float alpha;
  Float level;
};

struct Terms {
  Float feeds[FILTER::FEEDS];
  Float backs[FILTER::FEEDS];
};

using Design = auto (*)(const Angle &) -> Terms;

auto shelf(const Angle &angle, Float sign) -> Terms {
  const Float rise = angle.level + ONE, fall = angle.level - ONE;
  const Float lean = sign * fall * angle.cosine,
              sway = sign * rise * angle.cosine;
  const Float spread = TWO * std::sqrt(angle.level) * angle.alpha;
  return {
    {angle.level * (rise - lean + spread),
     TWO * sign * angle.level * (fall - sway),
     angle.level * (rise - lean - spread)},
    {rise + lean + spread, -TWO * sign * (fall + sway), rise + lean - spread}};
}

constexpr Design DESIGNS[] = {
  [](const Angle &angle) -> Terms {
    const Float side = (ONE - angle.cosine) / TWO;
    return {
      {side, ONE - angle.cosine, side},
      {ONE + angle.alpha, -TWO * angle.cosine, ONE - angle.alpha}};
  },
  [](const Angle &angle) -> Terms {
    const Float side = (ONE + angle.cosine) / TWO;
    return {
      {side, -ONE - angle.cosine, side},
      {ONE + angle.alpha, -TWO * angle.cosine, ONE - angle.alpha}};
  },
  [](const Angle &angle) -> Terms {
    return {
      {angle.alpha, 0, -angle.alpha},
      {ONE + angle.alpha, -TWO * angle.cosine, ONE - angle.alpha}};
  },
  [](const Angle &angle) -> Terms {
    return {
      {ONE, -TWO * angle.cosine, ONE},
      {ONE + angle.alpha, -TWO * angle.cosine, ONE - angle.alpha}};
  },
  [](const Angle &angle) -> Terms {
    const Float up = angle.alpha * angle.level,
                down = angle.alpha / angle.level;
    return {
      {ONE + up, -TWO * angle.cosine, ONE - up},
      {ONE + down, -TWO * angle.cosine, ONE - down}};
  },
  [](const Angle &angle) -> Terms { return shelf(angle, ONE); },
  [](const Angle &angle) -> Terms { return shelf(angle, -ONE); }};
static_assert(sizeof(DESIGNS) / sizeof(DESIGNS[0]) == FILTER::KINDS);

}  // namespace

void SOUND::PLUGINS::CORE::FILTER::settle(
  Biquad &biquad, Whole kind, Float cutoff, Float q, Float gain, Whole rate) {
  const Float omega = TURN * bounded(cutoff, rate) / Float(rate);
  const Float width = q < NARROWEST ? NARROWEST : q;
  const Angle angle = {
    std::cos(omega), std::sin(omega) / (TWO * width),
    std::pow(TEN, gain / DECIBELS)};
  const Terms terms = DESIGNS[kind < KINDS ? kind : LOW](angle);
  const Float scale = ONE / terms.backs[0];
  for (Whole at = 0; at < FEEDS; ++at)
    biquad.feeds[at] = terms.feeds[at] * scale;
  biquad.backs[0] = terms.backs[1] * scale;
  biquad.backs[1] = terms.backs[2] * scale;
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Biquad &biquad, Float in) -> Float {
  const Float out = biquad.feeds[0] * in + biquad.states[0];
  biquad.states[0] =
    biquad.feeds[1] * in - biquad.backs[0] * out + biquad.states[1];
  biquad.states[1] = biquad.feeds[2] * in - biquad.backs[1] * out;
  return out;
}
