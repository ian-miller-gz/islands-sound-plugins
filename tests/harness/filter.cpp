// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>

#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::HARNESS::FILTER {
void variable();
void vocal();
}  // namespace SOUND::PLUGINS::HARNESS::FILTER

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole RATE = 96000;
constexpr Whole TENTH = RATE / 10;
constexpr Whole HALF = RATE / 2;
constexpr Float TURN = 6.28318531f;
constexpr Float STEP = 0.01f;
constexpr Float IMPULSE = 0.1f;
constexpr Float CUTOFF = 1000;
constexpr Float LOW = 500;
constexpr Float HIGH = 5000;
constexpr Float BASS = 100;
constexpr Float MIDDLE = 0.5f;
constexpr Float FULL = 1;
constexpr Float DRIVE = 8;
constexpr Float HOT = 0.9f;

struct Setting {
  Float cutoff;
  Float emphasis;
  Float drive;
  Float bass;
};

constexpr Setting PLAIN = {CUTOFF, 0, FULL, 0};
constexpr Setting STOP = {LOW, 0, FULL, 0};
constexpr Setting HALVED = {CUTOFF, MIDDLE, FULL, 0};
constexpr Setting LIFTED = {CUTOFF, MIDDLE, FULL, FULL};
constexpr Setting RINGING = {CUTOFF, FULL, FULL, 0};
constexpr Setting DRIVEN = {CUTOFF, FULL, DRIVE, FULL};

auto step(Whole) -> Float { return STEP; }

auto impulse(Whole at) -> Float { return at == 0 ? IMPULSE : 0; }

auto high(Whole at) -> Float {
  return STEP * std::sin(TURN * HIGH * Float(at) / Float(RATE));
}

auto hot(Whole at) -> Float {
  return HOT * std::sin(TURN * BASS * Float(at) / Float(RATE));
}

template <class Source>
void print(
  STRING::Hot word, const Setting &setting, Source source, Whole from) {
  CORE::FILTER::Transistor filter;
  CORE::FILTER::settle(
    filter, setting.cutoff, setting.emphasis, setting.drive, setting.bass,
    RATE);
  Float peak = 0, square = 0, last = 0;
  Whole crossings = 0;
  for (Whole at = 0; at < RATE; ++at) {
    const Float out = CORE::FILTER::tick(filter, source(at));
    if (at < from) continue;
    crossings += at > from && (out < 0) != (last < 0);
    peak = std::fmax(peak, std::fabs(out));
    square += out * out;
    last = out;
  }
  const Float root = std::sqrt(square / Float(RATE - from));
  std::printf(
    "%s %.9g %.9g %zu %.9g\n", word, double(peak), double(root), crossings,
    double(last));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::FILTER {

void run() {
  ::print("dc", PLAIN, ::step, 0);
  ::print("stop", STOP, ::high, HALF);
  ::print("half", HALVED, ::step, 0);
  ::print("lifted", LIFTED, ::step, 0);
  ::print("ring", RINGING, ::impulse, RATE - TENTH);
  ::print("driven", DRIVEN, ::hot, 0);
  variable();
  vocal();
}

}  // namespace SOUND::PLUGINS::HARNESS::FILTER
