// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../line/line.hpp"
#include "../phase/phase.hpp"

namespace SOUND::CORE::PITCH {

constexpr Float THRESHOLD = 0.15f;
constexpr Whole POINTS = 1024;
constexpr Float SHORTEST = 16.0f;
constexpr Float ENVELOPE = 0.004f;

struct Estimate {
  Float hertz = 0;
  Float confidence = 0;
};

struct Detector {
  Vector<Float> ring;
  Vector<Float> samples;
  Vector<Float> differences;
  Whole span = 0;
  Whole lags = 0;
  Whole least = 0;
  Whole hop = 0;
  Whole head = 0;
  Whole count = 0;
  Whole rate = 0;
  Float threshold = THRESHOLD;
  Estimate estimate;
};

struct Grains {
  LINE::Line line;
  Vector<Float> weights;
  Float size = 0;
  Float phase = 0;
  Float step = 0;
  Float ratio = 1;
};

struct Formant {
  Grains envelope;
  Grains pitch;
};

void build(
  Detector &detector, Whole rate, Float lowest, Float highest, Whole hop);
auto feed(Detector &detector, const Float *samples, Whole frames) -> Flag;
void analyse(Detector &detector);

void build(Grains &grains, Float seconds, Whole rate);
void settle(Grains &grains, Float cents, Float seconds, Whole rate);
auto tick(Grains &grains, Float in) -> Float;

void build(Formant &formant, Float seconds, Whole rate);
void settle(
  Formant &formant, Float pitch, Float shift, Float seconds, Whole rate);
auto tick(Formant &formant, Float in) -> Float;

}  // namespace SOUND::CORE::PITCH
