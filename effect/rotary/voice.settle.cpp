// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void build(ROTARY::Rotor &rotor, const ROTARY::Build &build, Whole rate) {
  const Float seconds = ROTARY::MARGIN + build.reach * Float(ROTARY::SIDES);
  CORE::LINE::build(rotor.line, Whole(std::ceil(seconds * Float(rate))));
  for (Whole side = 0; side < ROTARY::SIDES; ++side) {
    CORE::LINE::settle(
      rotor.sweeps[side], ROTARY::MARGIN + build.reach, build.reach, 0, rate);
    rotor.lfos[side].wave = CORE::MODULATOR::SINE;
    rotor.lfos[side].start = ROTARY::HALF * Float(side);
    CORE::MODULATOR::settle(rotor.lfos[side], rate);
    CORE::MODULATOR::reset(rotor.lfos[side]);
  }
}

void settle(
  ROTARY::Rotor &rotor, const ROTARY::Build &build, const Float *rows,
  Whole rate) {
  const Float fast = rows[build.row];
  rotor.target = rows[ROTARY::SPEED] > 0 ? fast : fast * ROTARY::CHORALE;
  rotor.speed.time = rows[ROTARY::INERTIA] * build.heft;
  CORE::MODULATOR::settle(rotor.speed, rate);
  const Float closeness =
    ROTARY::NEAR / (ROTARY::NEAR + rows[ROTARY::DISTANCE]);
  rotor.depth = build.shade * closeness;
}

}  // namespace

void SOUND::ROTARY::build(Effect &effect) {
  for (Whole at = 0; at < ROTORS; ++at)
    ::build(effect.rotors[at], BUILDS[at], effect.rate);
}

void SOUND::ROTARY::settle(Effect &effect) {
  const Float cutoff = effect.rows[CROSSOVER];
  for (Whole stage = 0; stage < ORDER; ++stage) {
    CORE::FILTER::settle(
      effect.highs[stage], CORE::FILTER::HIGH, cutoff, CORE::FILTER::FLAT, 0,
      effect.rate);
    CORE::FILTER::settle(
      effect.lows[stage], CORE::FILTER::LOW, cutoff, CORE::FILTER::FLAT, 0,
      effect.rate);
  }
  for (Whole at = 0; at < ROTORS; ++at)
    ::settle(effect.rotors[at], BUILDS[at], effect.rows, effect.rate);
}
