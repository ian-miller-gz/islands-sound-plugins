// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "glottis.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float CORNER = 500.0f;
constexpr Float OCTAVES = 4.0f;
constexpr Float FALL = 0.5f;

}  // namespace

void SOUND::PLUGINS::CORE::GLOTTIS::settle(
  Source &source, Float tension, Float breath, Whole rate) {
  const Float held = FILTER::bounded(tension);
  const Float open = WIDEST - held * (WIDEST - CLOSEST);
  settle(source, open, breath * (FULL - FALL * held));
  const Float corner = CORNER * std::exp2(OCTAVES * held);
  FILTER::settle(source.tilt, FILTER::LOW, corner, rate);
  source.tilted = true;
  source.closed = 0;
}
