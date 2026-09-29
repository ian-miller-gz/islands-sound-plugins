// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <iterator>

#include "voice.internal.hpp"

void SOUND::PARAMETRIC::build(Effect &effect) {
  effect.strips.resize(effect.channels);
}

void SOUND::PARAMETRIC::settle(Effect &effect) {
  for (Whole band = 0; band < BANDS; ++band) {
    CORE::FILTER::Biquad design;
    CORE::FILTER::settle(
      design, KINDS[band], effect.rows[FREQUENCY + band], effect.rows[Q + band],
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

auto SOUND::PARAMETRIC::tick(Strip &strip, Float in) -> Float {
  Float out = in;
  for (auto &band : strip.bands) out = CORE::FILTER::tick(band, out);
  return out;
}
