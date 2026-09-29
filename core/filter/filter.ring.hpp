// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::CORE::FILTER {

constexpr Float STABLE = 0.999f;

struct Ring {
  Vector<Float> cells;
  Whole head = 0;
};

void build(Ring &ring, Whole frames);
void push(Ring &ring, Float value);
auto read(const Ring &ring, Float delay) -> Float;

struct Comb {
  Ring ring;
  Float delay = 1;
  Float feedback = 0;
  Float damp = 0;
  Float held = 0;
};

void build(Comb &comb, Whole frames);
void settle(Comb &comb, Float delay, Float feedback, Float damp);
auto tick(Comb &comb, Float in) -> Float;

struct Allpass {
  Ring ring;
  Float delay = 1;
  Float gain = 0;
};

void build(Allpass &allpass, Whole frames);
void settle(Allpass &allpass, Float delay, Float gain);
auto tick(Allpass &allpass, Float in) -> Float;

}  // namespace SOUND::CORE::FILTER
