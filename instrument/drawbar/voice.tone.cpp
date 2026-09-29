// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LEVEL = 0.08f;
constexpr Float FULL = 1;
constexpr Float DRIVING = 8;

struct Tones {
  Float body = 0;
  Float struck = 0;
};

auto turn(DRAWBAR::Organ &organ) -> Tones {
  Tones tones;
  for (Whole wheel = 0; wheel < DRAWBAR::WHEELS; ++wheel) {
    Float &level = organ.levels[wheel];
    level += (organ.targets[wheel] - level) * organ.pole;
    const Float value = CORE::OSCILLATOR::sine(organ.sine, organ.phases[wheel]);
    organ.phases[wheel] += organ.steps[wheel];
    tones.body += level * value;
    tones.struck += organ.strikes[wheel] * value;
  }
  return tones;
}

auto drive(const Float *rows, Float in) -> Float {
  const Float driven = rows[DRAWBAR::DRIVE];
  if (driven <= 0) return in;
  const Float gain = FULL + DRIVING * driven;
  return CORE::SHAPER::soft(in * gain) / CORE::SHAPER::soft(gain);
}

}  // namespace

auto SOUND::PLUGINS::DRAWBAR::sound(Organ &organ) -> Pair {
  const Tones tones = ::turn(organ);
  const Float decay = CORE::ENVELOPE::tick(organ.gate, organ.percussion);
  const Float percussion = tones.struck * decay * organ.accent;
  Float mixed = LEVEL * (tones.body + percussion);
  mixed += CORE::NOISE::tick(organ.click);
  mixed = tick(organ.scanner, mixed);
  return tick(organ.rotary, organ, ::drive(organ.rows, mixed));
}
