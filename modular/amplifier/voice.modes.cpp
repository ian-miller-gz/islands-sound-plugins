// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Shape = auto (*)(AMPLIFIER::Module &module, const AMPLIFIER::Tap &tap)
  -> Float;

constexpr Float UNITY = 1.0f;
constexpr Float LOWEST = 40.0f;
constexpr Float SWEEP = 500.0f;

auto opened(Float control) -> Float { return control < 0 ? 0 : control; }

auto cutoff(Float control) -> Float {
  const Float bounded = control < UNITY ? control : UNITY;
  return LOWEST * std::pow(SWEEP, ::opened(bounded));
}

auto clamp(AMPLIFIER::Module &, const AMPLIFIER::Tap &tap) -> Float {
  return tap.sample * ::opened(tap.control);
}

auto ring(AMPLIFIER::Module &, const AMPLIFIER::Tap &tap) -> Float {
  return tap.sample * tap.control;
}

auto lowpass(AMPLIFIER::Module &module, const AMPLIFIER::Tap &tap) -> Float {
  CORE::FILTER::Variable &filter = module.filters[tap.channel];
  if (tap.due)
    CORE::FILTER::settle(
      filter, CORE::FILTER::LOW, ::cutoff(tap.control), 0, module.rate);
  return CORE::FILTER::tick(filter, ::clamp(module, tap));
}

constexpr Shape SHAPES[] = {clamp, ring, lowpass};
static_assert(std::size(SHAPES) == AMPLIFIER::MODES);

}  // namespace

auto SOUND::PLUGINS::AMPLIFIER::amplify(Module &module, const Tap &tap)
  -> Float {
  const Whole mode = Whole(module.rows[MODE]);
  const Shape shape = SHAPES[mode < MODES ? mode : CLAMP];
  return shape(module, tap) * module.rows[GAIN];
}
