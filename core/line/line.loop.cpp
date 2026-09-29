// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "line.hpp"

namespace {
using namespace SOUND;

constexpr Float TAU = 2.0f * std::numbers::pi_v<Float>;
constexpr Float NYQUIST = 0.5f;

auto smoothing(Float seconds, Whole rate) -> Float {
  const Float frames = seconds * Float(rate);
  if (frames <= CORE::LINE::UNITY) return CORE::LINE::UNITY;
  return CORE::LINE::UNITY - std::exp(-CORE::LINE::UNITY / frames);
}

auto pole(Float cutoff, Whole rate) -> Float {
  if (rate == 0 || cutoff <= 0) return 0;
  const Float top = NYQUIST * Float(rate);
  const Float bounded = cutoff > top ? top : cutoff;
  return std::exp(-TAU * bounded / Float(rate));
}

}  // namespace

void SOUND::CORE::LINE::settle(
  Sweep &sweep, Float centre, Float depth, Float glide, Whole rate) {
  sweep.centre = centre * Float(rate);
  sweep.depth = depth * Float(rate);
  sweep.glide = ::smoothing(glide, rate);
  if (sweep.delay < NEAREST) sweep.delay = sweep.centre;
}

auto SOUND::CORE::LINE::read(const Line &line, Sweep &sweep, Float modulation)
  -> Float {
  const Float target = sweep.centre + sweep.depth * modulation;
  sweep.delay += (target - sweep.delay) * sweep.glide;
  return read(line, sweep.delay);
}

void SOUND::CORE::LINE::settle(
  Loop &loop, Float feedback, Float cutoff, Whole rate) {
  loop.feedback = feedback;
  loop.pole = ::pole(cutoff, rate);
}

auto SOUND::CORE::LINE::damp(Loop &loop, Float value) -> Float {
  loop.held = value + (loop.held - value) * loop.pole;
  return loop.held;
}

auto SOUND::CORE::LINE::tick(Line &line, Float in, Float delay) -> Float {
  const Float out = read(line, delay);
  write(line, in);
  return out;
}

auto SOUND::CORE::LINE::tick(Line &line, Loop &loop, Float in, Float delay)
  -> Float {
  const Float out = read(line, delay);
  write(line, in + loop.feedback * damp(loop, out));
  return out;
}

auto SOUND::CORE::LINE::tick(
  Line &line, Loop &loop, Float in, Sweep &sweep, Float modulation) -> Float {
  const Float out = read(line, sweep, modulation);
  write(line, in + loop.feedback * damp(loop, out));
  return out;
}
