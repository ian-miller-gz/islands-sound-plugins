// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "../../core/modulator/modulator.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Float SECONDS = 0.4f;
constexpr Whole RATE = 3000;
constexpr Whole SPAN = 60;
constexpr Whole LAG = 120;
constexpr CORE::MODULATOR::Seed FIRST = 1;
constexpr CORE::MODULATOR::Seed SECOND = 2;

auto walk(CORE::MODULATOR::Seed state) -> Vector<Float> {
  CORE::MODULATOR::Drift drift;
  CORE::MODULATOR::seed(drift, state);
  CORE::MODULATOR::settle(drift, SECONDS, RATE);
  Vector<Float> values(SPAN * RATE);
  for (Float &value : values) value = CORE::MODULATOR::tick(drift);
  return values;
}

void spread(const Vector<Float> &values) {
  double most = 0, square = 0;
  for (const Float value : values) {
    most = std::fmax(most, std::fabs(double(value)));
    square += double(value) * double(value);
  }
  std::printf("most %.9g\n", most);
  std::printf("rms %.9g\n", std::sqrt(square / double(values.size())));
}

void correlate(const Vector<Float> &values) {
  double mean = 0;
  for (const Float value : values) mean += double(value);
  mean /= double(values.size());
  double whole = 0, lagged = 0;
  for (Whole at = 0; at < values.size(); ++at) {
    const double here = double(values[at]) - mean;
    whole += here * here;
    if (at + LAG < values.size())
      lagged += here * (double(values[at + LAG]) - mean);
  }
  std::printf("correlation %.9g\n", lagged / whole);
}

void repeat(const Vector<Float> &values) {
  const Vector<Float> again = ::walk(FIRST);
  Whole differing = 0;
  for (Whole at = 0; at < values.size(); ++at)
    differing +=
      std::bit_cast<Bits>(values[at]) != std::bit_cast<Bits>(again[at]);
  std::printf("differing %zu\n", differing);
}

void part(const Vector<Float> &values) {
  CORE::MODULATOR::Drift drift;
  CORE::MODULATOR::seed(drift, SECOND);
  CORE::MODULATOR::settle(drift, SECONDS, RATE);
  const Float other = CORE::MODULATOR::tick(drift);
  const Flag apart =
    std::bit_cast<Bits>(values[0]) != std::bit_cast<Bits>(other);
  std::printf("first %.9g %.9g %d\n", double(values[0]), double(other), apart);
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::MODULATOR {

void run() {
  const Vector<Float> values = ::walk(FIRST);
  ::spread(values);
  ::correlate(values);
  ::repeat(values);
  ::part(values);
}

}  // namespace SOUND::PLUGINS::HARNESS::MODULATOR
