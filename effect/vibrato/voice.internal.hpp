// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "vibrato.hpp"
#include "../../core/block/block.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::VIBRATO {

constexpr Float MARGIN = 0.001f;
constexpr Float GLIDE = 0.002f;
constexpr Float DOUBLE = 2.0f;
constexpr Float UNITY = 1.0f;

struct Channel {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweep;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Channel> strips;
  Float rows[PARAMETERS] = {};
  CORE::MODULATOR::Lfo lfo;
  CORE::BLOCK::Meter meter;
};

auto excursion(Float hertz, Float cents) -> Float;
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::VIBRATO
