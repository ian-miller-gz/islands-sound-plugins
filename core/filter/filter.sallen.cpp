// SPDX-License-Identifier: AGPL-3.0-or-later
#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float LEAST = 0.01f;
constexpr Float MOST = 2.0f;

enum Place : Whole { INPUT, FORWARD, RETURN };

constexpr Whole ROUTES[][FILTER::LOOPS] = {
  {FILTER::LOW, FILTER::LOW, FILTER::HIGH},
  {FILTER::HIGH, FILTER::HIGH, FILTER::LOW}};
constexpr Whole SIDES = sizeof(ROUTES) / sizeof(ROUTES[0]);

auto spill(const FILTER::Pole &pole) -> Float {
  return (ONE - pole.gain) * pole.state;
}

auto low(FILTER::Sallen &sallen, Float in) -> Float {
  FILTER::Pole *poles = sallen.poles;
  const Float first = FILTER::tick(poles[INPUT], in);
  const Float gain = poles[FORWARD].gain;
  const Float sum = sallen.feedback * (ONE - gain) * ::spill(poles[FORWARD]) -
                    ::spill(poles[RETURN]);
  const Float node = SHAPER::soft(sallen.scale * (first + sum));
  const Float out = sallen.feedback * FILTER::tick(poles[FORWARD], node);
  FILTER::tick(poles[RETURN], out);
  return out / sallen.feedback;
}

auto high(FILTER::Sallen &sallen, Float in) -> Float {
  FILTER::Pole *poles = sallen.poles;
  const Float first = FILTER::tick(poles[INPUT], in);
  const Float gain = poles[FORWARD].gain;
  const Float sum = ::spill(poles[RETURN]) - gain * ::spill(poles[FORWARD]);
  const Float node = SHAPER::soft(sallen.scale * (first + sum));
  const Float out = sallen.feedback * node;
  FILTER::tick(poles[RETURN], FILTER::tick(poles[FORWARD], out));
  return node;
}

using Flow = auto (*)(FILTER::Sallen &, Float) -> Float;

constexpr Flow FLOWS[] = {low, high};
static_assert(sizeof(FLOWS) / sizeof(FLOWS[0]) == SIDES);

}  // namespace

void SOUND::PLUGINS::CORE::FILTER::settle(
  Sallen &sallen, Whole kind, Float cutoff, Float emphasis, Float drive,
  Whole rate) {
  sallen.kind = kind < SIDES ? kind : LOW;
  for (Whole at = 0; at < LOOPS; ++at)
    settle(sallen.poles[at], ROUTES[sallen.kind][at], cutoff, rate);
  const Float gain = sallen.poles[0].gain;
  sallen.feedback = LEAST + bounded(emphasis) * (MOST - LEAST);
  sallen.scale = ONE / (ONE - sallen.feedback * gain * (ONE - gain));
  sallen.drive = drive;
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Sallen &sallen, Float in) -> Float {
  return FLOWS[sallen.kind](sallen, sallen.drive * in);
}
