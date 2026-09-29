// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "echo.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::ECHO {

constexpr Float MILLISECOND = 0.001f;
constexpr Float REACH = 0.002f;
constexpr Float SHARE = 0.05f;
constexpr Float WOBBLE = 0.7f;
constexpr Float WARBLE = 7.5f;
constexpr Float INERTIA = 0.2f;
constexpr Float DRIVES = 3.0f;
constexpr Float CUT = 60.0f;
constexpr Float BRIGHTEST = 12000.0f;
constexpr Float DARKEST = 1500.0f;
constexpr Float UNITY = 1.0f;

struct Track {
  CORE::LINE::Line line;
  CORE::LINE::Loop loop;
  CORE::FILTER::Pole cut;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Track> tracks;
  CORE::MODULATOR::Lfo wow;
  CORE::MODULATOR::Lfo flutter;
  CORE::MODULATOR::Smoother time;
  Float frames = 0;
  Float reach = 0;
  Float drive = UNITY;
  Float rows[PARAMETERS] = {};
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

auto tick(Track &track, Float in, Float delay, Float drive) -> Float;

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ECHO
