// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds * PINGPONG::MILLISECOND * Float(rate);
}

}  // namespace

void SOUND::PLUGINS::PINGPONG::build(Effect &effect) {
  const Float longest =
    ::frames(CORE::TABLE::found(SHEET, TIME)->most, effect.rate);
  for (auto &line : effect.lines)
    CORE::LINE::build(line, Whole(std::ceil(longest)) + 1);
  effect.time.time = INERTIA;
  CORE::MODULATOR::settle(effect.time, effect.rate);
  CORE::MODULATOR::jump(effect.time, ::frames(effect.rows[TIME], effect.rate));
}

void SOUND::PLUGINS::PINGPONG::settle(Effect &effect) {
  effect.frames = ::frames(effect.rows[TIME], effect.rate);
  effect.feedback = effect.rows[FEEDBACK];
  effect.width = effect.rows[WIDTH];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}

auto SOUND::PLUGINS::PINGPONG::bounce(Effect &effect, Float in) -> Stereo {
  const Float at = CORE::MODULATOR::tick(effect.time, effect.frames);
  const Float left = CORE::LINE::read(effect.lines[LEFT], at);
  const Float right = CORE::LINE::read(effect.lines[RIGHT], at);
  CORE::LINE::write(effect.lines[LEFT], in + effect.feedback * right);
  CORE::LINE::write(effect.lines[RIGHT], effect.feedback * left);
  const Float middle = (left + right) * HALF;
  const Float side = (left - right) * HALF * effect.width;
  return {middle + side, middle - side};
}
