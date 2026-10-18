// SPDX-License-Identifier: AGPL-3.0-or-later
#include "line.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float THIRD = 1.0f / 3.0f;
constexpr Float HALF = 0.5f;
constexpr Float SLOWEST = 0.05f;
constexpr Float FASTEST = 20.0f;
constexpr Float INSTANT = 0;

constexpr CORE::STEREO::Pair GAINS[CORE::LINE::READS] = {
  {CORE::LINE::UNITY, 0}, {HALF, HALF}, {0, CORE::LINE::UNITY}};

auto held(Float value, Float least, Float most) -> Float {
  return value > least ? (value < most ? value : most) : least;
}

}  // namespace

void SOUND::PLUGINS::CORE::LINE::build(Chorus &chorus, Whole rate) {
  build(chorus.line, Whole(LENGTH * Float(rate)));
  for (Whole at = 0; at < READS; ++at) {
    MODULATOR::Lfo &lfo = chorus.lfos[at];
    lfo.wave = MODULATOR::TRIANGLE;
    lfo.start = Float(at) * ::THIRD;
    MODULATOR::reset(lfo);
  }
}

void SOUND::PLUGINS::CORE::LINE::settle(
  Chorus &chorus, Float hertz, Float depth, Float wet, Whole rate) {
  const Float pace = ::held(hertz, ::SLOWEST, ::FASTEST);
  const Float deep = ::held(depth, 0, DEEPEST);
  for (Whole at = 0; at < READS; ++at) {
    chorus.lfos[at].hertz = pace;
    MODULATOR::settle(chorus.lfos[at], rate);
    settle(chorus.sweeps[at], CENTRE, deep, ::INSTANT, rate);
  }
  chorus.wet = ::held(wet, 0, UNITY);
}

auto SOUND::PLUGINS::CORE::LINE::tick(Chorus &chorus, Float in)
  -> STEREO::Pair {
  STEREO::Pair sum;
  Float whole = 0;
  for (Whole at = 0; at < READS; ++at) {
    const Float swing = MODULATOR::tick(chorus.lfos[at]);
    const Float out = read(chorus.line, chorus.sweeps[at], swing);
    sum.left += ::GAINS[at].left * out;
    sum.right += ::GAINS[at].right * out;
    whole += out;
  }
  write(chorus.line, in);
  const Float wet = chorus.wet;
  chorus.whole = in + wet * whole * ::THIRD;
  return {in + wet * sum.left, in + wet * sum.right};
}
