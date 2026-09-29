// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "cloud.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float PI = std::numbers::pi_v<Float>;
constexpr Float UNITY = 1.0f;

auto weighed(const CLOUD::Cloud &cloud, Float phase) -> Float {
  const Float place = phase * Float(CLOUD::POINTS);
  const Whole whole = Whole(place);
  if (whole >= CLOUD::POINTS) return cloud.window.back();
  const Float part = place - Float(whole);
  const Float near = cloud.window[whole];
  return near + (cloud.window[whole + 1] - near) * part;
}

auto heard(
  const CLOUD::Cloud &cloud, CLOUD::Grain &grain,
  const LINE::Line &line) -> Float {
  const Float weight = ::weighed(cloud, grain.age / grain.span);
  const Float out = grain.gain * weight * LINE::read(line, grain.delay);
  grain.delay += grain.step;
  grain.age += UNITY;
  if (grain.age >= grain.span) grain.live = false;
  return out;
}

}  // namespace

void SOUND::CORE::CLOUD::build(Cloud &cloud, Whole count) {
  cloud.grains.assign(count, Grain{});
  cloud.window.resize(POINTS + 1);
  for (Whole point = 0; point <= POINTS; ++point) {
    const Float sine = std::sin(PI * Float(point) / Float(POINTS));
    cloud.window[point] = sine * sine;
  }
}

void SOUND::CORE::CLOUD::clear(Cloud &cloud) {
  for (Grain &grain : cloud.grains) grain.live = false;
}

auto SOUND::CORE::CLOUD::start(Cloud &cloud, const Grain &grain) -> Flag {
  if (grain.span < UNITY) return false;
  for (Grain &slot : cloud.grains) {
    if (slot.live) continue;
    slot = grain;
    slot.age = 0;
    slot.live = true;
    return true;
  }
  return false;
}

auto SOUND::CORE::CLOUD::tick(Cloud &cloud, const LINE::Line &line) -> Float {
  Float sum = 0;
  for (Grain &grain : cloud.grains)
    if (grain.live) sum += ::heard(cloud, grain, line);
  return sum;
}
