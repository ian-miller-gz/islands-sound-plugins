// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SEAM = 0.01f;
constexpr Whole SHORTEST = 2;
constexpr Float WIDTH = 0.7f;
constexpr Float FLAT = 0;
constexpr Float UNISON = 1.0f;
constexpr Whole PAIR = 2;
constexpr Float HISS = 0.3f;
constexpr Float SEMITONE = 100.0f;

auto ring(PERCUSSION::Metal &metal) -> Float {
  Float sum = 0;
  for (Whole at = 0; at + 1 < metal.count; at += PAIR)
    sum += OSCILLATOR::tick(metal.oscillators[at], OSCILLATOR::PULSE) *
           OSCILLATOR::tick(metal.oscillators[at + 1], OSCILLATOR::PULSE);
  return sum;
}

void draw(
  Vector<Float> &samples, const PERCUSSION::HYBRID::Colour &colour,
  Whole rate) {
  PERCUSSION::Metal metal;
  PERCUSSION::settle(
    metal, PERCUSSION::METALLIC, PERCUSSION::BANK, UNISON, rate);
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Biquad high;
  FILTER::settle(band, FILTER::BAND, colour.band, WIDTH, FLAT, rate);
  FILTER::settle(high, FILTER::HIGH, colour.edge, FILTER::FLAT, FLAT, rate);
  Float peak = 0;
  for (Float &sample : samples) {
    const Float bank = ::ring(metal) + NOISE::tick(white) * HISS;
    sample = FILTER::tick(high, FILTER::tick(band, bank));
    const Float size = BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  if (peak > 0)
    for (Float &sample : samples) sample /= peak;
}

void seal(Vector<Float> &samples, Whole length) {
  const Whole seam = samples.size() - length;
  for (Whole at = 0; at < seam; ++at) {
    const Float in = Float(at) / Float(seam);
    samples[at] = samples[at] * in + samples[length + at] * (UNISON - in);
  }
  samples.resize(length);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::build(
  Loop &loop, Float seconds, const Colour &colour, Whole rate) {
  const Whole length = Whole(seconds * Float(rate));
  const Whole seam = Whole(::SEAM * Float(rate));
  if (length < ::SHORTEST) return;
  loop.samples.assign(length + seam, 0);
  ::draw(loop.samples, colour, rate);
  ::seal(loop.samples, length);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(Loop &loop, Float tune) {
  loop.speed = PHASE::ratio(tune * ::SEMITONE);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(Loop &loop) {
  loop.at = 0;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Loop &loop) -> Float {
  const Whole length = loop.samples.size();
  if (length == 0) return 0;
  const Whole at = Whole(loop.at);
  const Float part = loop.at - Float(at);
  const Float here = loop.samples[at];
  const Float sample = here + (loop.samples[(at + 1) % length] - here) * part;
  loop.at += loop.speed;
  if (loop.at >= Float(length)) loop.at -= Float(length);
  return sample;
}
