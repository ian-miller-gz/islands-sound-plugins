// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <numbers>

#include "reverb.hpp"

namespace {
using namespace SOUND;

constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Whole FIRST = 0;
constexpr Whole SECOND = 1;
constexpr Whole LAST = 2;
constexpr Whole PROBES = 7;
constexpr Float OUTPUT = 0.6f;
constexpr Float TAU = 2.0f * std::numbers::pi_v<Float>;

struct Probe {
  Whole side;
  Whole stage;
  Float delay;
  Float sign;
};

constexpr Probe TABLE[CORE::REVERB::SIDES][PROBES] = {
  {{RIGHT, FIRST, 266, 1},
   {RIGHT, FIRST, 2974, 1},
   {RIGHT, SECOND, 1913, -1},
   {RIGHT, LAST, 1996, 1},
   {LEFT, FIRST, 1990, -1},
   {LEFT, SECOND, 187, -1},
   {LEFT, LAST, 1066, -1}},
  {{LEFT, FIRST, 353, 1},
   {LEFT, FIRST, 3627, 1},
   {LEFT, SECOND, 1228, -1},
   {LEFT, LAST, 2673, 1},
   {RIGHT, FIRST, 2111, -1},
   {RIGHT, SECOND, 335, -1},
   {RIGHT, LAST, 121, -1}}};

auto staged(const CORE::REVERB::Half &half, Whole stage)
  -> const CORE::LINE::Line & {
  const CORE::LINE::Line *lines[] = {
    &half.first.line, &half.second.line, &half.last.line};
  return *lines[stage];
}

auto heard(const CORE::REVERB::Plate &plate, Whole side) -> Float {
  Float sum = 0;
  for (const Probe &probe : TABLE[side]) {
    const auto &line = ::staged(plate.halves[probe.side], probe.stage);
    sum += probe.sign * CORE::LINE::read(line, probe.delay * plate.scale);
  }
  return sum * OUTPUT;
}

void run(CORE::REVERB::Half &half, Float in, Float offset, Float decay) {
  const Float wobbled = CORE::REVERB::tick(half.wobble, in, offset);
  const Float delayed = CORE::REVERB::tick(half.first, wobbled);
  const Float damped = CORE::LINE::damp(half.loop, delayed) * decay;
  CORE::REVERB::tick(half.last, CORE::REVERB::tick(half.second, damped));
}

}  // namespace

auto SOUND::CORE::REVERB::tick(Plate &plate, Float in) -> Stereo {
  Float diffused = LINE::damp(plate.band, in);
  for (Allpass &diffuser : plate.diffusers) diffused = tick(diffuser, diffused);
  const Float offsets[SIDES] = {
    plate.excursion * std::sin(plate.phase),
    plate.excursion * std::cos(plate.phase)};
  plate.phase += plate.step;
  if (plate.phase >= TAU) plate.phase -= TAU;
  Float tails[SIDES];
  for (Whole side = 0; side < SIDES; ++side) {
    const Delay &last = plate.halves[side].last;
    tails[side] = LINE::read(last.line, last.delay);
  }
  for (Whole side = 0; side < SIDES; ++side) {
    const Float fed = diffused + plate.decay * tails[SIDES - 1 - side];
    ::run(plate.halves[side], fed, offsets[side], plate.decay);
  }
  return {::heard(plate, LEFT), ::heard(plate, RIGHT)};
}
