// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds * MULTITAP::MILLISECOND * Float(rate);
}

void place(MULTITAP::Tap &tap, Float level, Float pan) {
  const Float angle = (pan + MULTITAP::UNITY) * MULTITAP::QUARTER;
  tap.left = level * std::cos(angle);
  tap.right = level * std::sin(angle);
}

}  // namespace

void SOUND::MULTITAP::build(Effect &effect) {
  CORE::LINE::build(
    effect.line, Whole(std::ceil(::frames(LONGEST, effect.rate))) + 1);
  for (Whole at = 0; at < TAPS; ++at) {
    Tap &tap = effect.taps[at];
    tap.time.time = INERTIA;
    CORE::MODULATOR::settle(tap.time, effect.rate);
    CORE::MODULATOR::jump(
      tap.time, ::frames(effect.rows[TIME + at], effect.rate));
  }
}

void SOUND::MULTITAP::settle(Effect &effect) {
  effect.last = 0;
  for (Whole at = 0; at < TAPS; ++at) {
    Tap &tap = effect.taps[at];
    tap.frames = ::frames(effect.rows[TIME + at], effect.rate);
    ::place(tap, effect.rows[LEVEL + at], effect.rows[PAN + at]);
    if (tap.frames > effect.taps[effect.last].frames) effect.last = at;
  }
  effect.feedback = effect.rows[FEEDBACK];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
