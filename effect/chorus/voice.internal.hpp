// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chorus.hpp"
#include "../../core/block/block.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::CHORUS {

constexpr Float CENTRE = 0.015f;
constexpr Float GLIDE = 0.002f;
constexpr Float MILLISECOND = 0.001f;
constexpr Float UNITY = 1.0f;

struct Channel {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[MOST];
  CORE::MODULATOR::Lfo lfos[MOST];
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Channel> strips;
  Float rows[PARAMETERS] = {};
  Whole voices = FEWEST;
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Effect &effect);
void place(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CHORUS
