// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole PAIR = 2;

auto ceiling(const OSCILLATOR::Table &table, Whole level) -> Whole {
  const Whole length = OSCILLATOR::SIZE >> level;
  return table.levels == OSCILLATOR::SINGLE ? length / PAIR - 1
                                            : OSCILLATOR::TOP >> level;
}

void lay(Float *values, Whole length, const Float *partials, Whole count) {
  Float reals[OSCILLATOR::SIZE] = {};
  Float imags[OSCILLATOR::SIZE] = {};
  for (Whole harmonic = 1; harmonic <= count; ++harmonic)
    reals[harmonic] = partials[harmonic - 1];
  OSCILLATOR::transform(reals, imags, length);
  for (Whole at = 0; at < length; ++at) values[at] = imags[at];
  values[length] = values[0];
}

auto peak(const Float *values, Whole length) -> Float {
  Float most = 0;
  for (Whole at = 0; at < length; ++at)
    most = std::fmax(most, std::fabs(values[at]));
  return most;
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::draw(
  Table &table, Whole frame, const Float *partials, Whole count) {
  if (frame >= table.frames) return;
  Float *values = table.values.data() + frame * span(table.levels);
  for (Whole level = 0; level < table.levels; ++level) {
    const Whole most = ::ceiling(table, level);
    ::lay(
      values + offset(level), SIZE >> level, partials,
      count < most ? count : most);
  }
  const Float most = ::peak(values, SIZE);
  if (most <= 0) return;
  for (Whole at = 0; at < span(table.levels); ++at) values[at] /= most;
}

void SOUND::PLUGINS::CORE::OSCILLATOR::blend(
  Table &table, Whole from, Whole to) {
  if (to >= table.frames || to <= from) return;
  const Whole width = span(table.levels);
  Float *values = table.values.data();
  const Float *low = values + from * width;
  const Float *high = values + to * width;
  for (Whole frame = from + 1; frame < to; ++frame) {
    const Float part = Float(frame - from) / Float(to - from);
    Float *out = values + frame * width;
    for (Whole at = 0; at < width; ++at)
      out[at] = low[at] + (high[at] - low[at]) * part;
  }
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::level(const Table &table, Float step)
  -> Whole {
  for (Whole at = 0; at + 1 < table.levels; ++at)
    if (Float(TOP >> at) * step < NYQUIST) return at;
  return table.levels - 1;
}
