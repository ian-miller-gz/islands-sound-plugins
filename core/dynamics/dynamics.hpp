// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::CORE::DYNAMICS {

enum Kind : Whole { PEAK, RMS, KINDS };
enum Side : Whole { ABOVE, BELOW, SIDES };

constexpr Float SILENCE = -120.0f;
constexpr Float UNITY = 1.0f;

struct Detector {
  Float attack = 0;
  Float release = 0;
  Whole kind = PEAK;
  Float rise = UNITY;
  Float fall = UNITY;
  Float level = 0;
};

struct Computer {
  Float threshold = 0;
  Float ratio = UNITY;
  Float knee = 0;
  Whole side = ABOVE;
  Float factor = 0;
};

struct Ring {
  Vector<Float> samples;
  Whole length = 0;
  Whole at = 0;
};

struct Highpass {
  Float hertz = 0;
  Float pole = 0;
  Float low = 0;
};

struct Hold {
  Float time = 0;
  Whole length = 0;
  Whole left = 0;
  Float value = 0;
};

auto gain(Float decibels) -> Float;
auto decibels(Float gain) -> Float;

void settle(Detector &detector, Whole rate);
auto tick(Detector &detector, Float in) -> Float;

void settle(Computer &computer);
auto reduce(const Computer &computer, Float decibels) -> Float;

void delay(Ring &ring, Whole length);
auto tick(Ring &ring, Float in) -> Float;

void settle(Highpass &highpass, Whole rate);
auto tick(Highpass &highpass, Float in) -> Float;

void settle(Hold &hold, Whole rate);
auto tick(Hold &hold, Float in) -> Float;

}  // namespace SOUND::CORE::DYNAMICS

namespace SOUND::CORE::DYNAMICS::RING {
auto create(Whole capacity) -> Ring;
}  // namespace SOUND::CORE::DYNAMICS::RING
