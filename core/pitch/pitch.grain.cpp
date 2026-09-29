// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "pitch.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float PI = std::numbers::pi_v<Float>;
constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float BASE = CORE::LINE::NEAREST;

auto weighed(const CORE::PITCH::Grains &grains, Float phase) -> Float {
  const Float place = phase * Float(CORE::PITCH::POINTS);
  const Whole whole = Whole(place);
  if (whole >= CORE::PITCH::POINTS) return grains.weights.back();
  const Float part = place - Float(whole);
  const Float near = grains.weights[whole];
  return near + (grains.weights[whole + 1] - near) * part;
}

auto heard(const CORE::PITCH::Grains &grains, Float phase) -> Float {
  const Float delay = BASE + phase * grains.size;
  return CORE::LINE::read(grains.line, delay) * ::weighed(grains, phase);
}

auto wrapped(Float phase) -> Float { return phase - std::floor(phase); }

}  // namespace

void SOUND::PLUGINS::CORE::PITCH::build(
  Grains &grains, Float seconds, Whole rate) {
  const Float frames = seconds * Float(rate);
  LINE::build(grains.line, Whole(frames + BASE) + 1);
  grains.weights.resize(POINTS + 1);
  for (Whole point = 0; point <= POINTS; ++point) {
    const Float sine = std::sin(PI * Float(point) / Float(POINTS));
    grains.weights[point] = sine * sine;
  }
  grains.phase = 0;
}

void SOUND::PLUGINS::CORE::PITCH::settle(
  Grains &grains, Float cents, Float seconds, Whole rate) {
  const Float most = LINE::reach(grains.line) - BASE;
  const Float frames = seconds * Float(rate);
  grains.size = frames < SHORTEST ? SHORTEST : frames > most ? most : frames;
  grains.ratio = PHASE::ratio(cents);
  grains.step = (UNITY - grains.ratio) / grains.size;
}

auto SOUND::PLUGINS::CORE::PITCH::tick(Grains &grains, Float in) -> Float {
  if (grains.weights.empty()) return in;
  grains.phase = ::wrapped(grains.phase + grains.step);
  const Float other = ::wrapped(grains.phase + HALF);
  const Float out = ::heard(grains, grains.phase) + ::heard(grains, other);
  LINE::write(grains.line, in);
  return out;
}

void SOUND::PLUGINS::CORE::PITCH::build(
  Formant &formant, Float seconds, Whole rate) {
  build(formant.envelope, ENVELOPE, rate);
  build(formant.pitch, seconds, rate);
}

void SOUND::PLUGINS::CORE::PITCH::settle(
  Formant &formant, Float pitch, Float shift, Float seconds, Whole rate) {
  settle(formant.envelope, shift - pitch, ENVELOPE, rate);
  settle(formant.pitch, pitch, seconds, rate);
}

auto SOUND::PLUGINS::CORE::PITCH::tick(Formant &formant, Float in) -> Float {
  return tick(formant.pitch, tick(formant.envelope, in));
}
