// SPDX-License-Identifier: AGPL-3.0-or-later
#include "reverb.hpp"

namespace {
using namespace SOUND;

constexpr Whole COUNT = CORE::REVERB::TAPS * CORE::REVERB::SIDES;

constexpr CORE::LINE::Tap TABLE[COUNT] = {
  {0.0043f, 0.841f}, {0.0215f, 0.504f}, {0.0225f, 0.491f}, {0.0268f, 0.379f},
  {0.0270f, 0.380f}, {0.0298f, 0.346f}, {0.0458f, 0.289f}, {0.0485f, 0.272f},
  {0.0572f, 0.192f}, {0.0587f, 0.193f}, {0.0595f, 0.217f}, {0.0612f, 0.181f},
  {0.0707f, 0.180f}, {0.0708f, 0.181f}, {0.0726f, 0.176f}, {0.0741f, 0.142f},
  {0.0753f, 0.167f}, {0.0797f, 0.134f}};

constexpr Float LATEST = TABLE[COUNT - 1].delay;

auto placed(const CORE::LINE::Tap &tap, Float scale) -> CORE::LINE::Tap {
  return {tap.delay * scale, tap.gain};
}

}  // namespace

void SOUND::CORE::REVERB::build(Reflections &reflections, Whole rate) {
  reflections.rate = rate;
  const Float most = LATEST * LARGEST * Float(rate);
  LINE::build(reflections.line, Whole(most) + 1);
}

void SOUND::CORE::REVERB::settle(Reflections &reflections, Float size) {
  const Float scale = clamped(size) * Float(reflections.rate);
  for (Whole tap = 0; tap < TAPS; ++tap) {
    reflections.left[tap] = ::placed(TABLE[tap * SIDES], scale);
    reflections.right[tap] = ::placed(TABLE[tap * SIDES + 1], scale);
  }
}

auto SOUND::CORE::REVERB::tick(Reflections &reflections, Float in) -> Stereo {
  const Stereo out = {
    LINE::read(reflections.line, reflections.left, TAPS),
    LINE::read(reflections.line, reflections.right, TAPS)};
  LINE::write(reflections.line, in);
  return out;
}
