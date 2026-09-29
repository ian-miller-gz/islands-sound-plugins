// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "reverb.hpp"

namespace {
using namespace SOUND;

constexpr Float RATE = 29761.0f;
constexpr Float TAU = 2.0f * std::numbers::pi_v<Float>;
constexpr Float UNITY = 1.0f;

constexpr Float ENTRIES[CORE::REVERB::DIFFUSERS] = {142, 107, 379, 277};
constexpr Float SPREADS[CORE::REVERB::DIFFUSERS] = {
  0.75f, 0.75f, 0.625f, 0.625f};

struct Lengths {
  Float wobble;
  Float first;
  Float second;
  Float last;
};

constexpr Lengths HALVES[CORE::REVERB::SIDES] = {
  {672, 4453, 1800, 3720}, {908, 4217, 2656, 3163}};

constexpr Float EXCURSION = 16;
constexpr Float SPIN = 1.0f;
constexpr Float WOBBLE = 0.70f;
constexpr Float LIFT = 0.15f;
constexpr Float FLOOR = 0.25f;
constexpr Float CEILING = 0.50f;

auto frames(Float length, Float scale) -> Float { return length * scale; }

void build(CORE::REVERB::Allpass &allpass, Float frames, Float room) {
  CORE::REVERB::build(allpass, Whole(frames + room) + 1);
  allpass.delay = frames;
}

void build(CORE::REVERB::Delay &delay, Float frames) {
  CORE::REVERB::build(delay, Whole(frames) + 1);
  delay.delay = frames;
}

void build(CORE::REVERB::Half &half, const Lengths &lengths, Float scale) {
  ::build(half.wobble, ::frames(lengths.wobble, scale), EXCURSION * scale);
  ::build(half.first, ::frames(lengths.first, scale));
  ::build(half.second, ::frames(lengths.second, scale), 0);
  ::build(half.last, ::frames(lengths.last, scale));
}

auto scaled(Float pole, Float scale) -> Float {
  return pole <= 0 ? 0 : std::pow(pole, UNITY / scale);
}

}  // namespace

void SOUND::CORE::REVERB::build(Plate &plate, Whole rate) {
  plate.rate = rate;
  plate.scale = Float(rate) / RATE;
  for (Whole at = 0; at < DIFFUSERS; ++at)
    ::build(plate.diffusers[at], ::frames(ENTRIES[at], plate.scale), 0);
  for (Whole side = 0; side < SIDES; ++side)
    ::build(plate.halves[side], HALVES[side], plate.scale);
  plate.excursion = EXCURSION * plate.scale;
  plate.step = rate == 0 ? 0 : TAU * SPIN / Float(rate);
  plate.phase = 0;
}

void SOUND::CORE::REVERB::settle(
  Plate &plate, Float decay, Float damping, Float bandwidth, Float diffusion) {
  plate.decay = decay;
  plate.band.pole = ::scaled(UNITY - bandwidth, plate.scale);
  for (Whole at = 0; at < DIFFUSERS; ++at)
    plate.diffusers[at].gain = SPREADS[at] * diffusion;
  const Float lifted = decay + LIFT;
  const Float second = lifted < FLOOR     ? FLOOR
                       : lifted > CEILING ? CEILING
                                          : lifted;
  for (Half &half : plate.halves) {
    half.wobble.gain = -WOBBLE;
    half.second.gain = second;
    half.loop.pole = ::scaled(damping, plate.scale);
  }
}
