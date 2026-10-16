// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr OSCILLATOR::Wheel ONE = 1;

auto start(const OSCILLATOR::Table &table, Whole frame, Whole level)
  -> const Float * {
  const Whole last = table.frames == 0 ? 0 : table.frames - 1;
  const Whole held = frame < last ? frame : last;
  return table.values.data() + held * OSCILLATOR::span(table.levels) +
         OSCILLATOR::offset(level);
}

auto top(const OSCILLATOR::Table &table, Whole level) -> Whole {
  return level < table.levels ? level : table.levels - 1;
}

}  // namespace

void SOUND::PLUGINS::CORE::OSCILLATOR::build(Table &table, Whole frames) {
  build(table, frames, SINGLE);
}

void SOUND::PLUGINS::CORE::OSCILLATOR::build(
  Table &table, Whole frames, Whole levels) {
  table.frames = frames;
  table.levels = levels < SINGLE ? SINGLE : levels > LEVELS ? LEVELS : levels;
  table.values.assign(frames * span(table.levels), 0);
}

void SOUND::PLUGINS::CORE::OSCILLATOR::build(Table &table) {
  constexpr Float FUNDAMENTAL[] = {UNIT};
  build(table, SINGLE);
  draw(table, 0, FUNDAMENTAL, std::size(FUNDAMENTAL));
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::span(Whole levels) -> Whole {
  return offset(levels);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::offset(Whole level) -> Whole {
  Whole sum = 0;
  for (Whole at = 0; at < level; ++at) sum += (SIZE >> at) + 1;
  return sum;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::read(
  const Table &table, Whole frame, Wheel phase) -> Float {
  return read(table, frame, 0, phase);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::read(
  const Table &table, Whole frame, Whole level, Wheel phase) -> Float {
  if (table.frames == 0) return 0;
  const Whole held = ::top(table, level);
  const Float *values = ::start(table, frame, held);
  const Whole bits = SHIFT + held;
  const Wheel mask = (::ONE << bits) - 1;
  const Whole at = phase >> bits;
  const Float part = Float(phase & mask) / Float(::ONE << bits);
  return values[at] + (values[at + 1] - values[at]) * part;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::morph(
  const Table &table, Float position, Wheel phase) -> Float {
  return morph(table, position, 0, phase);
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::morph(
  const Table &table, Float position, Whole level, Wheel phase) -> Float {
  const Float top = table.frames == 0 ? 0 : Float(table.frames - 1);
  const Float held = position < 0 ? 0 : position > top ? top : position;
  const Whole low = Whole(held);
  const Float part = held - Float(low);
  const Float from = read(table, low, level, phase);
  return from + (read(table, low + 1, level, phase) - from) * part;
}

auto SOUND::PLUGINS::CORE::OSCILLATOR::sine(const Table &table, Wheel phase)
  -> Float {
  return read(table, 0, phase);
}
