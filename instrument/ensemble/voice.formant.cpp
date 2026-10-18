// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LOW = 450;
constexpr Float MIDDLE = 1100;
constexpr Float HIGH = 2600;
constexpr Float NARROW = 3;
constexpr Float BOOST = 9;

constexpr Float CENTRES[] = {LOW, MIDDLE, HIGH};
static_assert(std::size(CENTRES) == ENSEMBLE::BANDS);

}  // namespace

void SOUND::PLUGINS::ENSEMBLE::settle(
  Resonator &resonator, Float formant, Whole rate) {
  for (Whole at = 0; at < BANDS; ++at)
    CORE::FILTER::settle(
      resonator.bands[at], CORE::FILTER::PEAK, ::CENTRES[at], ::NARROW, ::BOOST,
      rate);
  resonator.wet = formant;
}

auto SOUND::PLUGINS::ENSEMBLE::tick(Resonator &resonator, Float in) -> Float {
  Float lifts = 0;
  for (CORE::FILTER::Biquad &band : resonator.bands)
    lifts += CORE::FILTER::tick(band, in) - in;
  return in + resonator.wet * lifts;
}
