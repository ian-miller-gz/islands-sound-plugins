// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "multitap.hpp"
#include "../../core/block/block.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::MULTITAP {

constexpr Whole MONO = 1;
constexpr Whole SIDES = 2;
constexpr Float MILLISECOND = 0.001f;
constexpr Float INERTIA = 0.1f;
constexpr Float QUARTER = 0.78539816f;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Stereo {
  Float left = 0;
  Float right = 0;
};

struct Tap {
  CORE::MODULATOR::Smoother time;
  Float frames = 0;
  Float left = 0;
  Float right = 0;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::LINE::Line line;
  Tap taps[TAPS];
  Whole last = 0;
  Float feedback = 0;
  Float rows[PARAMETERS] = {};
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

auto tick(Effect &effect, Float in) -> Stereo;

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::MULTITAP
