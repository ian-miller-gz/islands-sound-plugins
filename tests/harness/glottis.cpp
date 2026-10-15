// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "../../core/glottis/glottis.hpp"

namespace SOUND::PLUGINS::HARNESS::GLOTTIS {
void kept(const CORE::OSCILLATOR::Table &table);
}  // namespace SOUND::PLUGINS::HARNESS::GLOTTIS

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Whole RATE = 48000;
constexpr Float HERTZ = 110;
constexpr Float LAX = 0;
constexpr Float MIDDLE = 0.5f;
constexpr Float PRESSED = 1;
constexpr Float FULL = 1;
constexpr CORE::NOISE::Register OTHER = 0x2545F491u;

auto bits(Float value) -> Bits { return std::bit_cast<Bits>(value); }

void edge() {
  CORE::GLOTTIS::Source source;
  CORE::GLOTTIS::settle(source, MIDDLE, 0, RATE);
  const Float turn = Float(source.edge) / CORE::PHASE::TURN;
  std::printf("edge %.9g\n", double(turn));
}

auto brightness(const CORE::OSCILLATOR::Table &table, Float tension) -> Float {
  CORE::GLOTTIS::Source source;
  CORE::GLOTTIS::settle(source, tension, 0, RATE);
  CORE::GLOTTIS::tune(source, HERTZ, RATE);
  Float last = 0, square = 0, change = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Float out = CORE::GLOTTIS::tick(source, table);
    square += out * out;
    change += (out - last) * (out - last);
    last = out;
  }
  return std::sqrt(change / square);
}

void voice(CORE::GLOTTIS::Source &source) {
  CORE::GLOTTIS::settle(source, MIDDLE, FULL, RATE);
  CORE::GLOTTIS::tune(source, HERTZ, RATE);
}

void breathe(const CORE::OSCILLATOR::Table &table) {
  CORE::GLOTTIS::Source first, second;
  CORE::GLOTTIS::seed(second, OTHER);
  ::voice(first);
  ::voice(second);
  Whole closed = 0, shut = 0, open = 0, apart = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Flag inside = first.phase < first.edge;
    const Bits one = ::bits(CORE::GLOTTIS::tick(first, table));
    const Bits two = ::bits(CORE::GLOTTIS::tick(second, table));
    open += inside;
    apart += inside && one != two;
    closed += !inside;
    shut += !inside && one != two;
  }
  std::printf("breath %zu %zu %zu %zu\n", closed, shut, open, apart);
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::GLOTTIS {

void run() {
  CORE::OSCILLATOR::Table table;
  CORE::GLOTTIS::build(table);
  ::edge();
  const Float dark = ::brightness(table, LAX);
  const Float bright = ::brightness(table, PRESSED);
  std::printf("bright %.9g %.9g\n", double(dark), double(bright));
  ::breathe(table);
  kept(table);
}

}  // namespace SOUND::PLUGINS::HARNESS::GLOTTIS
