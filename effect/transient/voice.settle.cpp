// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void pace(
  CORE::DYNAMICS::Detector &detector, Float attack, Float release, Whole rate) {
  detector.kind = CORE::DYNAMICS::RMS;
  detector.attack = attack;
  detector.release = release;
  CORE::DYNAMICS::settle(detector, rate);
}

}  // namespace

void SOUND::TRANSIENT::build(Effect &effect) {
  ::pace(effect.quick, QUICK::ATTACK, QUICK::RELEASE, effect.rate);
  ::pace(effect.slow, SLOW::ATTACK, SLOW::RELEASE, effect.rate);
}

void SOUND::TRANSIENT::settle(Effect &) {}
