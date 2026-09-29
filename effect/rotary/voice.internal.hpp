// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "rotary.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::ROTARY {

constexpr Whole SIDES = 2;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Whole MONO = 1;
constexpr Whole ORDER = 2;
constexpr Whole ROTORS = 2;
constexpr Float CHORALE = 0.12f;
constexpr Float MARGIN = 0.001f;
constexpr Float NEAR = 0.3f;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Build {
  Whole row;
  Float reach;
  Float shade;
  Float heft;
};

inline constexpr Build BUILDS[ROTORS] = {
  {HORN, 0.0005f, 0.9f, 0.2f}, {DRUM, 0.0003f, 0.5f, 1.0f}};

struct Rotor {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[SIDES];
  CORE::MODULATOR::Lfo lfos[SIDES];
  CORE::MODULATOR::Smoother speed;
  Float target = 0;
  Float depth = 0;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::FILTER::Biquad highs[ORDER];
  CORE::FILTER::Biquad lows[ORDER];
  Rotor rotors[ROTORS];
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::ROTARY
