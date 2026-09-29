// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void copy(const CORE::FILTER::Biquad &design, CORE::FILTER::Biquad &into) {
  std::copy(
    std::begin(design.feeds), std::end(design.feeds), std::begin(into.feeds));
  std::copy(
    std::begin(design.backs), std::end(design.backs), std::begin(into.backs));
}

void shape(
  MULTIBAND::Split &split, const CORE::FILTER::Biquad &low,
  const CORE::FILTER::Biquad &high) {
  for (Whole stage = 0; stage < MULTIBAND::ORDER; ++stage) {
    ::copy(low, split.lows[stage]);
    ::copy(high, split.highs[stage]);
  }
}

void cross(MULTIBAND::Effect &effect, Whole at) {
  const Float cutoff = effect.rows[MULTIBAND::CROSSOVER + at];
  CORE::FILTER::Biquad low;
  CORE::FILTER::Biquad high;
  CORE::FILTER::settle(
    low, CORE::FILTER::LOW, cutoff, CORE::FILTER::FLAT, 0, effect.rate);
  CORE::FILTER::settle(
    high, CORE::FILTER::HIGH, cutoff, CORE::FILTER::FLAT, 0, effect.rate);
  for (MULTIBAND::Strip &strip : effect.strips) {
    ::shape(strip.splits[at], low, high);
    if (at == MULTIBAND::LAST) ::shape(strip.allpass, low, high);
  }
}

void settle(MULTIBAND::Band &band, const Float *rows, Whole at, Whole rate) {
  band.detector.attack = rows[MULTIBAND::ATTACK + at] * MULTIBAND::MILLI;
  band.detector.release = rows[MULTIBAND::RELEASE + at] * MULTIBAND::MILLI;
  CORE::DYNAMICS::settle(band.detector, rate);
  band.computer.threshold = rows[MULTIBAND::THRESHOLD + at];
  band.computer.ratio = rows[MULTIBAND::RATIO + at];
  band.computer.knee = MULTIBAND::KNEE;
  CORE::DYNAMICS::settle(band.computer);
  band.makeup = CORE::DYNAMICS::gain(rows[MULTIBAND::GAIN + at]);
}

}  // namespace

void SOUND::MULTIBAND::build(Effect &effect) {
  effect.strips.resize(effect.channels);
  for (Band &band : effect.bands) {
    band.detector.kind = CORE::DYNAMICS::PEAK;
    band.computer.side = CORE::DYNAMICS::ABOVE;
  }
}

void SOUND::MULTIBAND::settle(Effect &effect) {
  for (Whole at = 0; at < CROSSOVERS; ++at) ::cross(effect, at);
  for (Whole at = 0; at < BANDS; ++at)
    ::settle(effect.bands[at], effect.rows, at, effect.rate);
}
