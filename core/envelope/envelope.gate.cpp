// SPDX-License-Identifier: AGPL-3.0-or-later
#include "envelope.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float UP = 1.0f;
constexpr Float DOWN = -1.0f;
constexpr Float STILL = 0;

constexpr Float DIRECTIONS[ENVELOPE::STAGES] = {STILL, UP, DOWN, STILL, DOWN};

constexpr Whole NEXT[ENVELOPE::STAGES] = {
  ENVELOPE::IDLE, ENVELOPE::FALLING, ENVELOPE::HELD, ENVELOPE::HELD,
  ENVELOPE::IDLE};

constexpr Flag LIVE[ENVELOPE::STAGES] = {false, true, true, true, false};

auto following(const ENVELOPE::Envelope &envelope, Whole stage) -> Whole {
  const Whole next = NEXT[stage];
  return next == ENVELOPE::HELD && !envelope.sustained ? ENVELOPE::LEAVING
                                                       : next;
}

}  // namespace

void SOUND::CORE::ENVELOPE::strike(
  Gate &gate, const Envelope &envelope, Float velocity) {
  if (envelope.trigger != RESUME) gate.level = 0;
  gate.stage = RISING;
  gate.scale = weigh(velocity, envelope.depth);
}

void SOUND::CORE::ENVELOPE::tie(
  Gate &gate, const Envelope &envelope, Float velocity) {
  if (gate.stage < STAGES && LIVE[gate.stage]) return;
  strike(gate, envelope, velocity);
}

void SOUND::CORE::ENVELOPE::lift(Gate &gate) {
  if (gate.stage != IDLE) gate.stage = LEAVING;
}

void SOUND::CORE::ENVELOPE::choke(Gate &gate) {
  gate.level = 0;
  gate.stage = IDLE;
}

auto SOUND::CORE::ENVELOPE::sounding(const Gate &gate) -> Flag {
  return gate.stage != IDLE;
}

auto SOUND::CORE::ENVELOPE::tick(Gate &gate, const Envelope &envelope)
  -> Float {
  if (gate.stage >= STAGES) gate.stage = IDLE;
  const Slope &slope = envelope.slopes[gate.stage];
  gate.level = gate.level * slope.scale + slope.step;
  const Float bound = envelope.bounds[gate.stage];
  const Float direction = DIRECTIONS[gate.stage];
  if (direction != STILL && direction * (gate.level - bound) >= 0) {
    gate.level = bound;
    gate.stage = ::following(envelope, gate.stage);
  }
  return gate.level * gate.scale;
}
