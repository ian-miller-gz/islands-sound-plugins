// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>
#include <numbers>

#include "../../core/line/line.hpp"

namespace SOUND::PLUGINS::HARNESS::LINE {
auto correlation(const Vector<double> &left, const Vector<double> &right)
  -> double;
}  // namespace SOUND::PLUGINS::HARNESS::LINE

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole RATE = 48000;
constexpr Whole SPAN = 2 * RATE;
constexpr double TONE = 1000;
constexpr double TAU = 2 * std::numbers::pi;
constexpr Float SLOW = 0.513f;
constexpr Float WIDE = 0.00185f;
constexpr Float BEYOND = 0.01f;
constexpr Float FULL = 1;
constexpr double SIDES = 2;
constexpr double PARTS = 3;

auto sine(Whole at) -> Float {
  return Float(std::sin(TAU * TONE * double(at) / double(RATE)));
}

auto made(Float wet, Float depth) -> CORE::LINE::Chorus {
  CORE::LINE::Chorus chorus;
  CORE::LINE::build(chorus, RATE);
  CORE::LINE::settle(chorus, SLOW, depth, wet, RATE);
  return chorus;
}

void dry() {
  CORE::LINE::Chorus chorus = ::made(0, WIDE);
  Whole changed = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Float in = ::sine(at);
    const CORE::STEREO::Pair out = CORE::LINE::tick(chorus, in);
    changed += out.left != in || out.right != in || chorus.whole != in;
  }
  std::printf("dry %zu\n", changed);
}

void wet() {
  CORE::LINE::Chorus chorus = ::made(FULL, WIDE);
  Vector<double> left(SPAN), right(SPAN);
  double mono = 0, peak = 0;
  for (Whole at = 0; at < SPAN; ++at) {
    const double in = ::sine(at);
    const CORE::STEREO::Pair out = CORE::LINE::tick(chorus, Float(in));
    left[at] = out.left;
    right[at] = out.right;
    const double whole = in + (left[at] + right[at] - SIDES * in) / PARTS;
    mono = std::fmax(mono, std::fabs(chorus.whole - whole));
    peak =
      std::fmax(peak, std::fmax(std::fabs(left[at]), std::fabs(right[at])));
    peak = std::fmax(peak, std::fabs(chorus.whole));
  }
  std::printf("mono %.9g\npeak %.9g\n", mono, peak);
  std::printf("correlation %.9g\n", HARNESS::LINE::correlation(left, right));
}

void deep() {
  CORE::LINE::Chorus chorus = ::made(FULL, BEYOND);
  Float far = 0;
  for (Whole at = 0; at < SPAN; ++at) {
    CORE::LINE::tick(chorus, ::sine(at));
    for (const CORE::LINE::Sweep &sweep : chorus.sweeps)
      far = std::fmax(far, sweep.delay);
  }
  std::printf("deep");
  for (const CORE::LINE::Sweep &sweep : chorus.sweeps)
    std::printf(" %.9g", double(sweep.depth / Float(RATE)));
  const Float length = CORE::LINE::LENGTH * Float(RATE);
  const Float reach = CORE::LINE::reach(chorus.line);
  std::printf(
    "\nreach %.9g %.9g %.9g\n", double(far), double(length), double(reach));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::LINE {

void run() {
  ::dry();
  ::wet();
  ::deep();
}

}  // namespace SOUND::PLUGINS::HARNESS::LINE
