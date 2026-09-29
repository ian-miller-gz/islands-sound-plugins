// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "lofi.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::LOFI {

constexpr Float WOBBLE = 0.5f;
constexpr Float MARGIN = 0.001f;
constexpr Float GLIDE = 0.002f;
constexpr Float DOUBLE = 2.0f;
constexpr Float UNITY = 1.0f;
constexpr Float DECIBELS = 20.0f;
constexpr Float TEN = 10.0f;

struct Strip {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweep;
  CORE::FILTER::Biquad before;
  CORE::FILTER::Biquad after;
  CORE::SHAPER::Crusher crusher;
  CORE::NOISE::White white;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Strip> strips;
  Float rows[PARAMETERS] = {};
  CORE::MODULATOR::Lfo lfo;
  Float floor = 0;
  CORE::BLOCK::Meter meter;
};

auto excursion(Float cents) -> Float;
void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::LOFI
