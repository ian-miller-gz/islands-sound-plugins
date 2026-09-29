// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto swept(Float least, Float most, Float place) -> Float {
  return least * std::pow(most / least, place);
}

}  // namespace

void SOUND::PLUGINS::OVERDRIVE::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  for (auto &strip : effect.strips)
    CORE::FILTER::settle(strip.hump, CORE::FILTER::HIGH, HUMP, effect.rate);
}

void SOUND::PLUGINS::OVERDRIVE::settle(Effect &effect) {
  effect.gain = ::swept(GENTLEST, HOTTEST, effect.rows[DRIVE]);
  effect.level = CORE::DYNAMICS::gain(effect.rows[LEVEL]);
  const Float cutoff = ::swept(DARKEST, BRIGHTEST, effect.rows[TONE]);
  for (auto &strip : effect.strips)
    CORE::FILTER::settle(strip.tone, CORE::FILTER::LOW, cutoff, effect.rate);
}
