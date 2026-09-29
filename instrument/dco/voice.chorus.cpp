// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MILLISECOND = 0.001f;
constexpr Float SPAN = 12;
constexpr Float CENTRE = 3.5f;
constexpr Float WIDE = 1.85f;
constexpr Float NARROW = 0.2f;
constexpr Float SLOW = 0.513f;
constexpr Float FAST = 0.863f;
constexpr Float QUICK = 9.75f;
constexpr Float WET = 0.5f;
constexpr Float HALF = 0.5f;
constexpr Float THIRD = 1.0f / 3.0f;
constexpr Whole LEFT = 0;
constexpr Whole MIDDLE = 1;
constexpr Whole RIGHT = 2;
static_assert(RIGHT + 1 == DCO::LINES);

struct Mode {
  Float rate;
  Float depth;
  Float wet;
};

constexpr Mode MODES[] = {
  {SLOW, WIDE, 0}, {SLOW, WIDE, WET}, {FAST, WIDE, WET}, {QUICK, NARROW, WET}};
static_assert(std::size(MODES) == std::size(DCO::CHORUSES));

}  // namespace

void SOUND::PLUGINS::DCO::build(Chorus &chorus, Whole rate) {
  CORE::LINE::build(chorus.line, Whole(SPAN * MILLISECOND * Float(rate)));
  for (Whole at = 0; at < LINES; ++at) {
    CORE::MODULATOR::Lfo &lfo = chorus.lfos[at];
    lfo.wave = CORE::MODULATOR::TRIANGLE;
    lfo.start = Float(at) * THIRD;
    CORE::MODULATOR::reset(lfo);
  }
}

void SOUND::PLUGINS::DCO::settle(Chorus &chorus, Whole mode, Whole rate) {
  const Mode &chosen = ::MODES[mode < std::size(::MODES) ? mode : 0];
  for (Whole at = 0; at < LINES; ++at) {
    chorus.lfos[at].hertz = chosen.rate;
    CORE::MODULATOR::settle(chorus.lfos[at], rate);
    CORE::LINE::settle(
      chorus.sweeps[at], CENTRE * MILLISECOND, chosen.depth * MILLISECOND, 0,
      rate);
  }
  chorus.wet = chosen.wet;
}

auto SOUND::PLUGINS::DCO::tick(Chorus &chorus, Float in) -> Pair {
  Float wets[LINES];
  for (Whole at = 0; at < LINES; ++at) {
    const Float swing = CORE::MODULATOR::tick(chorus.lfos[at]);
    wets[at] = CORE::LINE::read(chorus.line, chorus.sweeps[at], swing);
  }
  CORE::LINE::write(chorus.line, in);
  const Float wet = chorus.wet;
  const Float middle = HALF * wets[MIDDLE];
  const Float sum = wets[LEFT] + wets[MIDDLE] + wets[RIGHT];
  return {
    in + wet * (wets[LEFT] + middle), in + wet * (wets[RIGHT] + middle),
    in + wet * sum * THIRD};
}
