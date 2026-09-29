// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto frames(Float milliseconds, Whole rate) -> Float {
  return milliseconds * ECHO::MILLISECOND * Float(rate);
}

auto cutoff(Float damp) -> Float {
  if (damp <= 0) return 0;
  return ECHO::BRIGHTEST * std::pow(ECHO::DARKEST / ECHO::BRIGHTEST, damp);
}

void wind(CORE::MODULATOR::Lfo &lfo, Float hertz, Whole rate) {
  lfo.wave = CORE::MODULATOR::SINE;
  lfo.hertz = hertz;
  CORE::MODULATOR::settle(lfo, rate);
}

}  // namespace

void SOUND::ECHO::build(Effect &effect) {
  const Float longest =
    ::frames(CORE::TABLE::found(SHEET, TIME)->most, effect.rate);
  const Float reach = REACH * (UNITY + SHARE) * Float(effect.rate);
  effect.tracks.resize(effect.channels);
  for (auto &track : effect.tracks) {
    CORE::LINE::build(track.line, Whole(std::ceil(longest + reach)) + 1);
    CORE::FILTER::settle(track.cut, CORE::FILTER::HIGH, CUT, effect.rate);
  }
  ::wind(effect.wow, WOBBLE, effect.rate);
  ::wind(effect.flutter, WARBLE, effect.rate);
  effect.time.time = INERTIA;
  CORE::MODULATOR::settle(effect.time, effect.rate);
  CORE::MODULATOR::jump(effect.time, ::frames(effect.rows[TIME], effect.rate));
  effect.reach = REACH * Float(effect.rate);
}

void SOUND::ECHO::settle(Effect &effect) {
  effect.frames = ::frames(effect.rows[TIME], effect.rate);
  const Float cutoff = ::cutoff(effect.rows[DAMP]);
  for (auto &track : effect.tracks)
    CORE::LINE::settle(track.loop, effect.rows[FEEDBACK], cutoff, effect.rate);
  effect.wow.depth = effect.rows[WOW];
  effect.flutter.depth = effect.rows[FLUTTER] * SHARE;
  effect.drive = UNITY + DRIVES * effect.rows[SATURATION];
  effect.dry = UNITY - effect.rows[MIX];
  effect.wet = effect.rows[MIX];
}
