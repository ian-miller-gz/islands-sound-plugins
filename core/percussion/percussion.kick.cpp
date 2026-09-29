// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float ONE = 1.0f;
constexpr Float BEND = 1.5f;
constexpr Float SWEEP = 0.03f;
constexpr Float SNAP = 0.005f;
constexpr Float CRISP = 2000.0f;
constexpr Float DRIVE = 8.0f;

}  // namespace

void SOUND::CORE::PERCUSSION::settle(
  Kick &kick, Float tune, Float decay, Float attack, Float drive, Float tone,
  Whole rate) {
  settle(kick.body, tune, decay, rate);
  kick.base = kick.body.turn;
  shape(kick.sweep, SWEEP, rate);
  shape(kick.snap, SNAP, rate);
  FILTER::settle(kick.crisp, FILTER::HIGH, CRISP, rate);
  FILTER::settle(kick.tone, FILTER::LOW, tone, rate);
  kick.attack = attack;
  kick.drive = ONE + drive * DRIVE;
}

void SOUND::CORE::PERCUSSION::strike(Kick &kick, Float velocity) {
  kick.velocity = struck(velocity);
  ENVELOPE::strike(kick.bend, kick.sweep, kick.velocity);
  ENVELOPE::strike(kick.click, kick.snap, kick.velocity);
  strike(kick.body, kick.velocity);
}

auto SOUND::CORE::PERCUSSION::tick(Kick &kick) -> Float {
  const Float sweep = ENVELOPE::tick(kick.bend, kick.sweep);
  kick.body.turn = kick.base * (ONE + BEND * sweep);
  const Float body = tick(kick.body);
  const Float snap = ENVELOPE::tick(kick.click, kick.snap);
  const Float noise = FILTER::tick(kick.crisp, NOISE::tick(kick.white));
  const Float crack = noise * snap * kick.attack * kick.velocity;
  const Float driven = SHAPER::soft((body + crack) * kick.drive);
  return FILTER::tick(kick.tone, driven);
}
