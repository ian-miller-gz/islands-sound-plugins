// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "pingpong.hpp"
#include "../../core/block/block.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PINGPONG {

constexpr Whole MONO = 1;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Whole SIDES = 2;
constexpr Float MILLISECOND = 0.001f;
constexpr Float INERTIA = 0.1f;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Stereo {
  Float left = 0;
  Float right = 0;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::LINE::Line lines[SIDES];
  CORE::MODULATOR::Smoother time;
  Float frames = 0;
  Float feedback = 0;
  Float width = UNITY;
  Float rows[PARAMETERS] = {};
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

auto bounce(Effect &effect, Float in) -> Stereo;

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PINGPONG
