// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float TAU = 2 * std::numbers::pi_v<Float>;
constexpr Float FULL = 1;
constexpr Float TWICE = 2;
constexpr Float CORNER = 2;
constexpr Float FRAME = 1;

auto lag(Float gain, Float omega) -> Float {
  const Float rise = gain * std::sin(omega);
  const Float run = FULL - gain * std::cos(omega);
  return FULL + TWICE * std::atan(rise / run) / omega;
}

auto most(Float room) -> Float {
  const Float share = room / Float(PLUCK::STAGES);
  const Float gain = share > FULL ? (share - FULL) / (share + FULL) : 0;
  return gain < CORE::FILTER::STABLE ? gain : CORE::FILTER::STABLE;
}

auto wanted(Float stiffness, Float omega) -> Float {
  if (stiffness <= 0) return 0;
  const Float gain = FULL - CORNER * omega / stiffness;
  return gain > 0 ? gain : 0;
}

}  // namespace

void SOUND::PLUGINS::PLUCK::build(Voice &voice) {
  for (CORE::FILTER::Allpass &stage : voice.stages) {
    CORE::FILTER::build(stage, Whole(FRAME));
    CORE::FILTER::settle(stage, FRAME, 0);
  }
}

auto SOUND::PLUGINS::PLUCK::stiffen(
  Voice &voice, Float stiffness, Float period, Float room) -> Float {
  const Float omega = TAU / period;
  const Float want = ::wanted(stiffness, omega), bound = ::most(room);
  const Float gain = want < bound ? want : bound;
  voice.engaged = gain > 0 ? STAGES : 0;
  for (CORE::FILTER::Allpass &stage : voice.stages)
    CORE::FILTER::settle(stage, FRAME, gain);
  return Float(voice.engaged) * ::lag(gain, omega);
}

auto SOUND::PLUGINS::PLUCK::disperse(Voice &voice, Float in) -> Float {
  Float out = in;
  for (Whole at = 0; at < voice.engaged; ++at)
    out = CORE::FILTER::tick(voice.stages[at], out);
  return out;
}
