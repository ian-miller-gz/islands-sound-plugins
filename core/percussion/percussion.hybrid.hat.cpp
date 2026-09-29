// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float LENGTH = 0.12f;
constexpr Float SEAM = 0.01f;
constexpr Whole SHORTEST = 2;
constexpr Float SHEEN = 8000.0f;
constexpr Float EDGE = 6000.0f;
constexpr Float WIDTH = 0.7f;
constexpr Float FLAT = 0;
constexpr Float UNISON = 1.0f;
constexpr Whole PAIR = 2;
constexpr Float HISS = 0.3f;
constexpr Float SEMITONE = 100.0f;
constexpr Float GAIN = 1.5f;

auto ring(PERCUSSION::Metal &metal) -> Float {
  Float sum = 0;
  for (Whole at = 0; at + 1 < metal.count; at += PAIR)
    sum += OSCILLATOR::tick(metal.oscillators[at], OSCILLATOR::PULSE) *
           OSCILLATOR::tick(metal.oscillators[at + 1], OSCILLATOR::PULSE);
  return sum;
}

void draw(Vector<Float> &loop, Whole rate) {
  PERCUSSION::Metal metal;
  PERCUSSION::settle(
    metal, PERCUSSION::METALLIC, PERCUSSION::BANK, UNISON, rate);
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Biquad high;
  FILTER::settle(band, FILTER::BAND, SHEEN, WIDTH, FLAT, rate);
  FILTER::settle(high, FILTER::HIGH, EDGE, FILTER::FLAT, FLAT, rate);
  Float peak = 0;
  for (Float &sample : loop) {
    const Float bank = ::ring(metal) + NOISE::tick(white) * HISS;
    sample = FILTER::tick(high, FILTER::tick(band, bank));
    const Float size = BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  if (peak > 0)
    for (Float &sample : loop) sample /= peak;
}

void seal(Vector<Float> &loop, Whole length) {
  const Whole seam = loop.size() - length;
  for (Whole at = 0; at < seam; ++at) {
    const Float in = Float(at) / Float(seam);
    loop[at] = loop[at] * in + loop[length + at] * (UNISON - in);
  }
  loop.resize(length);
}

}  // namespace

void SOUND::CORE::PERCUSSION::HYBRID::build(Hat &hat, Whole rate) {
  const Whole length = Whole(LENGTH * Float(rate));
  const Whole seam = Whole(SEAM * Float(rate));
  if (length < SHORTEST) return;
  hat.loop.assign(length + seam, 0);
  ::draw(hat.loop, rate);
  ::seal(hat.loop, length);
}

void SOUND::CORE::PERCUSSION::HYBRID::settle(
  Hat &hat, Float tune, Float closed, Float open, Whole rate) {
  hat.speed = PHASE::ratio(tune * SEMITONE);
  shape(hat.envelopes[CLOSED], closed, rate);
  shape(hat.envelopes[OPEN], open, rate);
}

void SOUND::CORE::PERCUSSION::HYBRID::strike(
  Hat &hat, Whole opening, Float velocity) {
  hat.opening = opening < OPENINGS ? opening : CLOSED;
  hat.velocity = struck(velocity);
  hat.at = 0;
  ENVELOPE::strike(hat.gate, hat.envelopes[hat.opening], hat.velocity);
}

auto SOUND::CORE::PERCUSSION::HYBRID::tick(Hat &hat) -> Float {
  const Whole length = hat.loop.size();
  if (length == 0 || !ENVELOPE::sounding(hat.gate)) return 0;
  const Whole at = Whole(hat.at);
  const Float part = hat.at - Float(at);
  const Float here = hat.loop[at];
  const Float sample = here + (hat.loop[(at + 1) % length] - here) * part;
  hat.at += hat.speed;
  if (hat.at >= Float(length)) hat.at -= Float(length);
  const Float level = ENVELOPE::tick(hat.gate, hat.envelopes[hat.opening]);
  return SHAPER::soft(sample * level * hat.velocity * GAIN);
}
