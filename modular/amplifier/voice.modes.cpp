// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Shape = auto (*)(AMPLIFIER::Module &module, const AMPLIFIER::Tap &tap)
  -> Float;

constexpr Float UNITY = 1.0f;
constexpr Float LOWEST = 40.0f;
constexpr Float SWEEP = 500.0f;
constexpr Float SPAN = 60.0f;

auto opened(Float control) -> Float { return control < 0 ? 0 : control; }

auto taper(Float level, Float span) -> Float {
  const Float size = level < 0 ? -level : level;
  const Float under = size < UNITY ? size : UNITY;
  return level * CORE::DYNAMICS::gain(span * (under - UNITY));
}

auto shaped(const AMPLIFIER::Module &module, Float control) -> Float {
  const Float response = module.rows[AMPLIFIER::RESPONSE];
  return control + response * (::taper(control, SPAN) - control);
}

auto cutoff(Float control) -> Float {
  const Float bounded = control < UNITY ? control : UNITY;
  return LOWEST * std::pow(SWEEP, ::opened(bounded));
}

auto clamp(AMPLIFIER::Module &module, const AMPLIFIER::Tap &tap) -> Float {
  return tap.sample * ::shaped(module, ::opened(tap.control));
}

auto ring(AMPLIFIER::Module &module, const AMPLIFIER::Tap &tap) -> Float {
  return tap.sample * ::shaped(module, tap.control);
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
