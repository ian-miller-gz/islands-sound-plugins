// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1.0f;
constexpr Float REACH = 0.02f;
constexpr CORE::NOISE::Register SEEDING = 0x2545F491u;

auto fraction(CORE::MODULATOR::Seed &seed) -> Float {
  return (CORE::MODULATOR::draw(seed) + FULL) * HALF;
}

void seat(CHOIR::Choir &choir, CHOIR::Singer &singer, Whole at) {
  CORE::MODULATOR::Seed &seed = choir.seed;
  CORE::GLOTTIS::seed(
    singer.source,
    CORE::NOISE::SEED ^ (CORE::NOISE::Register(at + 1) * ::SEEDING));
  singer.seat.detune = CORE::MODULATOR::draw(seed);
  singer.seat.shift = CORE::MODULATOR::draw(seed);
  singer.seat.pan = CORE::MODULATOR::draw(seed);
  singer.seat.pace = CORE::MODULATOR::draw(seed);
  singer.vibrato.start = ::fraction(seed);
}

void space(CHOIR::Choir &choir) {
  const Whole frames = Whole(REACH * Float(choir.rate)) + 1;
  for (CHOIR::Side &side : choir.ensemble.sides)
    CORE::LINE::build(side.line, frames);
  for (Whole tap = 0; tap < CHOIR::TAPS; ++tap) {
    CORE::MODULATOR::Lfo &lfo = choir.ensemble.lfos[tap];
    lfo.start = Float(tap) / Float(CHOIR::TAPS);
    CORE::MODULATOR::reset(lfo);
  }
  CORE::REVERB::build(choir.room.network, CORE::REVERB::FOUR, choir.rate);
}

}  // namespace

void SOUND::PLUGINS::CHOIR::build(Choir &choir) {
  CORE::GLOTTIS::build(choir.table);
  choir.allocator.count = NOTES;
  Whole at = 0;
  for (Part &part : choir.parts)
    for (Singer &singer : part.singers) ::seat(choir, singer, at++);
  ::space(choir);
}
