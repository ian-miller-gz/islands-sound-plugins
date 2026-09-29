// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "pluck.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::PLUCK {

constexpr Whole VOICES = 16;
constexpr Whole BODIES = 2;

enum State : Whole { RINGING, DAMPED, STATES };

struct Voice {
  CORE::LINE::Line string;
  CORE::LINE::Line pick;
  CORE::LINE::Allpass allpass;
  CORE::LINE::Loop loop;
  CORE::NOISE::Burst burst;
  Float delay = 1;
  Float notch = 1;
  Float feedbacks[STATES] = {};
  Float level = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::FILTER::Biquad bodies[BODIES];
  CORE::SHAPER::Blocker blocker;
  Float tune = 0;
  Float fall = 0;
  CORE::BLOCK::Meter meter;
};

void build(Synth &synth);
void settle(Synth &synth);
void tune(Synth &synth, Whole at);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::PLUCK
