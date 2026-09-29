// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../line/line.hpp"

namespace SOUND::PLUGINS::CORE::REVERB {

constexpr Whole FOUR = 4;
constexpr Whole EIGHT = 8;
constexpr Whole DIFFUSERS = 4;
constexpr Whole SIDES = 2;
constexpr Whole TAPS = 9;
constexpr Float SMALLEST = 0.25f;
constexpr Float LARGEST = 2.0f;

struct Stereo {
  Float left = 0;
  Float right = 0;
};

struct Delay {
  LINE::Line line;
  Float delay = 0;
};

struct Comb {
  LINE::Line line;
  LINE::Loop loop;
  Float delay = 0;
};

struct Allpass {
  LINE::Line line;
  Float delay = 0;
  Float gain = 0;
};

struct Network {
  LINE::Line lines[EIGHT];
  LINE::Loop loops[EIGHT];
  Float delays[EIGHT] = {};
  Whole count = EIGHT;
  Whole rate = 0;
};

struct Reflections {
  LINE::Line line;
  LINE::Tap left[TAPS];
  LINE::Tap right[TAPS];
  Whole rate = 0;
};

struct Half {
  Allpass wobble;
  Delay first;
  LINE::Loop loop;
  Allpass second;
  Delay last;
};

struct Plate {
  Whole rate = 0;
  Float scale = 0;
  LINE::Loop band;
  Allpass diffusers[DIFFUSERS];
  Half halves[SIDES];
  Float decay = 0;
  Float excursion = 0;
  Float phase = 0;
  Float step = 0;
};

auto decay(Float frames, Float seconds, Whole rate) -> Float;
auto clamped(Float size) -> Float;

void build(Delay &delay, Whole frames);
void build(Comb &comb, Whole frames);
void build(Allpass &allpass, Whole frames);
void build(Network &network, Whole count, Whole rate);
void build(Reflections &reflections, Whole rate);
void build(Plate &plate, Whole rate);

void settle(Delay &delay, Float frames);
void settle(Comb &comb, Float frames, Float seconds, Float cutoff, Whole rate);
void settle(Allpass &allpass, Float frames, Float gain);
void settle(Network &network, Float size, Float seconds, Float cutoff);
void settle(Reflections &reflections, Float size);
void settle(
  Plate &plate, Float decay, Float damping, Float bandwidth, Float diffusion);

auto tick(Delay &delay, Float in) -> Float;
auto tick(Comb &comb, Float in) -> Float;
auto tick(Allpass &allpass, Float in) -> Float;
auto tick(Allpass &allpass, Float in, Float offset) -> Float;
auto tick(Network &network, Float in) -> Stereo;
auto tick(Reflections &reflections, Float in) -> Stereo;
auto tick(Plate &plate, Float in) -> Stereo;

}  // namespace SOUND::PLUGINS::CORE::REVERB
