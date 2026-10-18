// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>
#include <iterator>

#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {
using CORE::OSCILLATOR::Table;
using CORE::OSCILLATOR::Wheel;
constexpr Wheel QUARTER = Wheel(1) << 30;
auto tap(const Table &table, Whole level, Whole at) -> Float;
}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR

namespace {
using namespace SOUND::PLUGINS;
using HARNESS::OSCILLATOR::Table;

constexpr Whole FRAMES = 3;
constexpr Whole MIDDLE = 1;
constexpr Whole LAST = 2;
constexpr Whole PROBED = 3;
constexpr Float HALFWAY = 0.5f;
constexpr Float TWICE = 2.0f;
constexpr Float FUNDAMENTAL[] = {1.0f};
constexpr Float NONE[] = {0.0f};

auto weigh(const Table &table, Whole level, Whole harmonic) -> Float {
  const Whole length = CORE::OSCILLATOR::SIZE >> level;
  Float sum = 0;
  for (Whole at = 0; at < length; ++at) {
    const Float turns = Float(at * harmonic % length) / Float(length);
    const Float drawn = HARNESS::OSCILLATOR::tap(table, level, at);
    sum += drawn * std::sin(CORE::OSCILLATOR::TAU * turns);
  }
  return sum * TWICE / Float(length);
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {

auto tap(const Table &table, Whole level, Whole at) -> Float {
  const Whole bits = CORE::OSCILLATOR::SHIFT + level;
  return CORE::OSCILLATOR::read(table, 0, level, Wheel(at) << bits);
}

void content(const Table &table) {
  const Whole top = CORE::OSCILLATOR::TOP >> ::PROBED;
  std::printf(
    "content %.9g %.9g %.9g\n", double(::weigh(table, 0, top)),
    double(::weigh(table, ::PROBED, top)),
    double(::weigh(table, ::PROBED, top + 1)));
}

void blend() {
  Table table;
  CORE::OSCILLATOR::build(table, ::FRAMES, CORE::OSCILLATOR::LEVELS);
  CORE::OSCILLATOR::draw(table, 0, ::FUNDAMENTAL, std::size(::FUNDAMENTAL));
  CORE::OSCILLATOR::draw(table, ::LAST, ::NONE, std::size(::NONE));
  CORE::OSCILLATOR::blend(table, 0, ::LAST);
  std::printf(
    "blend %.9g %.9g %.9g\n",
    double(CORE::OSCILLATOR::read(table, ::MIDDLE, ::PROBED, QUARTER)),
    double(CORE::OSCILLATOR::morph(table, ::HALFWAY, ::PROBED, QUARTER)),
    double(CORE::OSCILLATOR::read(table, ::LAST, 0, QUARTER)));
}

}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR
