// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float MILLISECOND = 0.001f;
constexpr Float SPAN = 4;
constexpr Float CENTRE = 1.5f;
constexpr Float SCAN = 6.86f;
constexpr Float SHALLOW = 0.25f;
constexpr Float MIDDLE = 0.5f;
constexpr Float DEEP = 1.0f;
constexpr Float FULL = 1;
constexpr Float HALF = 0.5f;

struct Mode {
  Float depth;
  Float wet;
  Float dry;
};

constexpr Mode MODES[] = {{0, 0, FULL},          {SHALLOW, FULL, 0},
                          {MIDDLE, FULL, 0},     {DEEP, FULL, 0},
                          {SHALLOW, HALF, HALF}, {MIDDLE, HALF, HALF},
                          {DEEP, HALF, HALF}};
static_assert(std::size(MODES) == std::size(DRAWBAR::SCANNERS));

}  // namespace

void SOUND::DRAWBAR::build(Scanner &scanner, Whole rate) {
  CORE::LINE::build(scanner.line, Whole(SPAN * MILLISECOND * Float(rate)));
  scanner.lfo.wave = CORE::MODULATOR::TRIANGLE;
  scanner.lfo.hertz = SCAN;
  CORE::MODULATOR::settle(scanner.lfo, rate);
  CORE::MODULATOR::reset(scanner.lfo);
}

void SOUND::DRAWBAR::settle(Scanner &scanner, Whole mode, Whole rate) {
  const Mode &chosen = ::MODES[mode < std::size(::MODES) ? mode : 0];
  CORE::LINE::settle(
    scanner.sweep, CENTRE * MILLISECOND, chosen.depth * MILLISECOND, 0, rate);
  scanner.wet = chosen.wet;
  scanner.dry = chosen.dry;
}

auto SOUND::DRAWBAR::tick(Scanner &scanner, Float in) -> Float {
  const Float swing = CORE::MODULATOR::tick(scanner.lfo);
  const Float wet = CORE::LINE::read(scanner.line, scanner.sweep, swing);
  CORE::LINE::write(scanner.line, in);
  return scanner.dry * in + scanner.wet * wet;
}
