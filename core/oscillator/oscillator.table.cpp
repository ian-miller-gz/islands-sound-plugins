// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <iterator>

#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;

auto start(const OSCILLATOR::Table &table, Whole frame) -> const Float * {
  const Whole last = table.frames == 0 ? 0 : table.frames - 1;
  return table.values.data() +
         (frame < last ? frame : last) * OSCILLATOR::STRIDE;
}

auto peak(const Float *values) -> Float {
  Float most = 0;
  for (Whole at = 0; at < OSCILLATOR::SIZE; ++at)
    most = std::fmax(most, std::fabs(values[at]));
  return most;
}

auto partial(Whole at, Whole harmonic) -> Float {
  const Float turns = Float(at * harmonic % OSCILLATOR::SIZE);
  return std::sin(OSCILLATOR::TAU * turns / Float(OSCILLATOR::SIZE));
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::build(Table &table, Whole frames) {
  table.frames = frames;
  table.values.assign(frames * STRIDE, 0);
}

void SOUND::PLUGINS::CORE::OSCILLATOR::build(Table &table) {
  constexpr Float FUNDAMENTAL[] = {UNIT};
  build(table, SINGLE);
  draw(table, 0, FUNDAMENTAL, std::size(FUNDAMENTAL));
}

void SOUND::PLUGINS::CORE::OSCILLATOR::draw(
  Table &table, Whole frame, const Float *partials, Whole count) {
  if (frame >= table.frames) return;
  Float *values = table.values.data() + frame * STRIDE;
  for (Whole at = 0; at < SIZE; ++at) {
    Float sum = 0;
    for (Whole harmonic = 0; harmonic < count; ++harmonic)
      sum += partials[harmonic] * ::partial(at, harmonic + 1);
    values[at] = sum;
  }
  const Float most = ::peak(values);
  if (most > 0)
    for (Whole at = 0; at < SIZE; ++at) values[at] /= most;
  values[SIZE] = values[0];
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::read(
  const Table &table, Whole frame, Wheel phase) -> Float {
  if (table.frames == 0) return 0;
  const Float *values = ::start(table, frame);
  const Whole at = phase >> SHIFT;
  const Float part = Float(phase & MASK) / SLICE;
  return values[at] + (values[at + 1] - values[at]) * part;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::morph(
  const Table &table, Float position, Wheel phase) -> Float {
  const Float top = table.frames == 0 ? 0 : Float(table.frames - 1);
  const Float held = position < 0 ? 0 : position > top ? top : position;
  const Whole low = Whole(held);
  const Float part = held - Float(low);
  const Float from = read(table, low, phase);
  return from + (read(table, low + 1, phase) - from) * part;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::sine(const Table &table, Wheel phase)
  -> Float {
  return read(table, 0, phase);
}
