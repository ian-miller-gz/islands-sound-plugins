// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "drawbar.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::DRAWBAR {

constexpr Whole WHEELS = 91;

struct Pair {
  Float left = 0;
  Float right = 0;
};

struct Scanner {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweep;
  CORE::MODULATOR::Lfo lfo;
  Float wet = 0;
  Float dry = 1;
};

struct Rotor {
  CORE::LINE::Line line;
  CORE::LINE::Sweep near;
  CORE::LINE::Sweep far;
  CORE::MODULATOR::Smoother speed;
  CORE::PHASE::Wheel phase = 0;
  Float target = 0;
  Float swing = 0;
};

struct Rotary {
  CORE::FILTER::Biquad low;
  CORE::FILTER::Biquad high;
  Rotor horn;
  Rotor drum;
  Flag spinning = false;
};

struct Organ {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::OSCILLATOR::Table sine;
  CORE::PHASE::Wheel phases[WHEELS] = {};
  CORE::PHASE::Wheel steps[WHEELS] = {};
  Float targets[WHEELS] = {};
  Float levels[WHEELS] = {};
  Float strikes[WHEELS] = {};
  Float gains[BARS] = {};
  Flag keys[CORE::PHASE::PITCHES] = {};
  Whole held = 0;
  Float pole = 1;
  Float accent = 0;
  CORE::ENVELOPE::Envelope percussion;
  CORE::ENVELOPE::Gate gate;
  CORE::NOISE::Burst click;
  Scanner scanner;
  Rotary rotary;
  CORE::BLOCK::Meter meter;
};

void tune(Organ &organ);
void pull(Organ &organ);
void settle(Organ &organ);
void apply(Organ &organ, const AUDIO::PLUGIN::Event &event);

void build(Scanner &scanner, Whole rate);
void settle(Scanner &scanner, Whole mode, Whole rate);
auto tick(Scanner &scanner, Float in) -> Float;

void build(Rotary &rotary, Whole rate);
void settle(Rotary &rotary, Whole mode, Whole rate);
auto tick(Rotary &rotary, const Organ &organ, Float in) -> Pair;

auto sound(Organ &organ) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::DRAWBAR
