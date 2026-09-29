// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float MOTOR = 20;
constexpr Float TEETH = 2;
constexpr Integer BOTTOM = 24;
constexpr Integer OCTAVE = 12;
constexpr Float DECIBELS = 20;
constexpr Float STEP = 3;
constexpr Float TEN = 10;
constexpr Integer SECOND = 12;
constexpr Integer THIRD = 19;
constexpr Whole TOP = DRAWBAR::BARS - 1;

struct Gear {
  Float driving;
  Float driven;
};

constexpr Gear GEARS[] = {{85, 104},  {71, 82}, {67, 73}, {105, 108},
                          {103, 100}, {84, 77}, {74, 64}, {98, 80},
                          {96, 74},   {88, 64}, {67, 46}, {108, 70}};
static_assert(std::size(GEARS) == Whole(OCTAVE));

constexpr Integer OFFSETS[] = {-12, 7, 0, 12, 19, 24, 28, 31, 36};
static_assert(std::size(OFFSETS) == DRAWBAR::BARS);

constexpr Integer HARMONICS[] = {SECOND, THIRD};

auto fold(Integer pitch) -> Whole {
  Integer wheel = pitch - BOTTOM;
  while (wheel < 0) wheel += OCTAVE;
  while (wheel >= Integer(DRAWBAR::WHEELS)) wheel -= OCTAVE;
  return Whole(wheel);
}

auto gain(Float degree) -> Float {
  if (degree <= 0) return 0;
  return std::pow(TEN, -STEP * (DRAWBAR::PULLED - degree) / DECIBELS);
}

void sound(DRAWBAR::Organ &organ, Integer pitch, Integer harmonic) {
  for (Whole bar = 0; bar < DRAWBAR::BARS; ++bar)
    organ.targets[::fold(pitch + OFFSETS[bar])] += organ.gains[bar];
  organ.strikes[::fold(pitch + harmonic)] += 1;
}

}  // namespace

void SOUND::DRAWBAR::tune(Organ &organ) {
  const Float cents = CORE::PHASE::ratio(organ.rows[TUNE]);
  for (Whole wheel = 0; wheel < WHEELS; ++wheel) {
    const Gear &gear = ::GEARS[wheel % Whole(::OCTAVE)];
    const Float octave = std::exp2(Float(wheel / Whole(::OCTAVE)));
    const Float hertz = MOTOR * gear.driving / gear.driven * TEETH * octave;
    organ.steps[wheel] = CORE::PHASE::step(hertz * cents, organ.rate);
  }
}

void SOUND::DRAWBAR::pull(Organ &organ) {
  const Flag percussive = organ.rows[PERCUSSION] > 0;
  for (Whole bar = 0; bar < BARS; ++bar)
    organ.gains[bar] = ::gain(organ.rows[bar]);
  if (percussive) organ.gains[::TOP] = 0;
  std::fill(std::begin(organ.targets), std::end(organ.targets), 0.0f);
  std::fill(std::begin(organ.strikes), std::end(organ.strikes), 0.0f);
  const Integer harmonic = ::HARMONICS[organ.rows[HARMONIC] > 0 ? 1 : 0];
  for (Whole pitch = 0; pitch < CORE::PHASE::PITCHES; ++pitch)
    if (organ.keys[pitch]) ::sound(organ, Integer(pitch), harmonic);
}
