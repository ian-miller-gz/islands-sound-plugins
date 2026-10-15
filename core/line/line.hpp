// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../modulator/modulator.hpp"
#include "../stereo/stereo.hpp"

namespace SOUND::PLUGINS::CORE::LINE {

constexpr Whole GUARD = 4;
constexpr Float NEAREST = 1.0f;
constexpr Float UNITY = 1.0f;
constexpr Float THIN = 0.1f;

constexpr Whole READS = 3;
constexpr Float CENTRE = 0.0035f;
constexpr Float DEEPEST = 0.003f;
constexpr Float LENGTH = 0.012f;

struct Line {
  Vector<Float> ring;
  Whole mask = 0;
  Whole head = 0;
};

struct Allpass {
  Float held = 0;
};

struct Tap {
  Float delay = 0;
  Float gain = 0;
};

struct Sweep {
  Float centre = 0;
  Float depth = 0;
  Float glide = UNITY;
  Float delay = 0;
};

struct Loop {
  Float feedback = 0;
  Float pole = 0;
  Float held = 0;
};

struct Chorus {
  Line line;
  Sweep sweeps[READS];
  MODULATOR::Lfo lfos[READS];
  Float wet = 0;
  Float whole = 0;
};

void build(Line &line, Whole frames);
void clear(Line &line);
auto reach(const Line &line) -> Float;
void write(Line &line, Float in);

auto read(const Line &line, Whole delay) -> Float;
auto read(const Line &line, Float delay) -> Float;
auto read(const Line &line, Float delay, Allpass &allpass) -> Float;
auto read(const Line &line, const Tap *taps, Whole count) -> Float;
auto read(const Line &line, Sweep &sweep, Float modulation) -> Float;

void settle(Sweep &sweep, Float centre, Float depth, Float glide, Whole rate);
void settle(Loop &loop, Float feedback, Float cutoff, Whole rate);
auto damp(Loop &loop, Float value) -> Float;

auto tick(Line &line, Float in, Float delay) -> Float;
auto tick(Line &line, Loop &loop, Float in, Float delay) -> Float;
auto tick(Line &line, Loop &loop, Float in, Sweep &sweep, Float modulation)
  -> Float;

void build(Chorus &chorus, Whole rate);
void settle(Chorus &chorus, Float hertz, Float depth, Float wet, Whole rate);
auto tick(Chorus &chorus, Float in) -> STEREO::Pair;

}  // namespace SOUND::PLUGINS::CORE::LINE
