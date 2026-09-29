// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <iterator>

#include "voice.internal.hpp"

void SOUND::GRAPHIC::build(Effect &effect) {
  effect.strips.resize(effect.channels);
}

void SOUND::GRAPHIC::settle(Effect &effect) {
  for (Whole band = 0; band < BANDS; ++band) {
    CORE::FILTER::Biquad design;
    CORE::FILTER::settle(
      design, CORE::FILTER::PEAK, CENTRES[band], OCTAVE,
      effect.rows[GAIN + band], effect.rate);
    for (auto &strip : effect.strips) {
      std::copy(
        std::begin(design.feeds), std::end(design.feeds),
        std::begin(strip.bands[band].feeds));
      std::copy(
        std::begin(design.backs), std::end(design.backs),
        std::begin(strip.bands[band].backs));
    }
  }
}

auto SOUND::GRAPHIC::tick(Strip &strip, Float in) -> Float {
  Float out = in;
  for (auto &band : strip.bands) out = CORE::FILTER::tick(band, out);
  return out;
}
