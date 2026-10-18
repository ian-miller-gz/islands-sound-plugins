// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>

#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {
using CORE::OSCILLATOR::Table;
using CORE::OSCILLATOR::Wheel;
constexpr Wheel QUARTER = Wheel(1) << 30;
auto tap(const Table &table, Whole level, Whole at) -> Float;
void content(const Table &table);
void blend();
}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR

namespace {
using namespace SOUND::PLUGINS;
using HARNESS::OSCILLATOR::Table;

constexpr Whole FRAMES = 3;
constexpr Float UNIT = 1.0f;
constexpr Float STEPS[] = {0.0009765625f, 0.001953125f, 0.01f, 0.4f, 0.6f};

void saw(Table &table) {
  Float partials[CORE::OSCILLATOR::TOP];
  for (Whole at = 0; at < CORE::OSCILLATOR::TOP; ++at)
    partials[at] = UNIT / Float(at + 1);
  CORE::OSCILLATOR::build(table, FRAMES, CORE::OSCILLATOR::LEVELS);
  CORE::OSCILLATOR::draw(table, 0, partials, CORE::OSCILLATOR::TOP);
}

auto direct(Whole at) -> Float {
  Float sum = 0;
  for (Whole harmonic = 1; harmonic <= CORE::OSCILLATOR::TOP; ++harmonic) {
    const Float turns = Float(at * harmonic % CORE::OSCILLATOR::SIZE);
    const Float angle = CORE::OSCILLATOR::TAU * turns / CORE::OSCILLATOR::SIZE;
    sum += std::sin(angle) / Float(harmonic);
  }
  return sum;
}

void error(const Table &table) {
  Float most = 0;
  Float worst = 0;
  for (Whole at = 0; at < CORE::OSCILLATOR::SIZE; ++at)
    most = std::fmax(most, std::fabs(::direct(at)));
  for (Whole at = 0; at < CORE::OSCILLATOR::SIZE; ++at) {
    const Float drawn = HARNESS::OSCILLATOR::tap(table, 0, at);
    worst = std::fmax(worst, std::fabs(::direct(at) / most - drawn));
  }
  std::printf("error %.9g\n", double(worst));
}

void single() {
  Table table;
  CORE::OSCILLATOR::build(table);
  const Float sine =
    CORE::OSCILLATOR::sine(table, HARNESS::OSCILLATOR::QUARTER);
  std::printf(
    "single %.9g %.9g %.9g\n", double(table.levels),
    double(table.values.size()), double(sine));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {

void levelled() {
  ::single();
  Table table;
  ::saw(table);
  std::printf(
    "levels %.9g %.9g\n", double(table.levels), double(table.values.size()));
  std::printf("level");
  for (const Float step : ::STEPS)
    std::printf(" %.9g", double(CORE::OSCILLATOR::level(table, step)));
  std::printf("\n");
  ::error(table);
  content(table);
  blend();
}

}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR
