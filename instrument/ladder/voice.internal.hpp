// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "ladder.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::LADDER {

enum Shape : Whole {
  TRIANGLE,
  SHARKTOOTH,
  REVERSE,
  SAW,
  SQUARE,
  WIDE,
  NARROW,
  SHAPES
};

constexpr Whole FIRST = 0;
constexpr Whole SECOND = 1;
constexpr Whole THIRD = 2;
constexpr Float REFERENCE = 60;

struct Source {
  CORE::OSCILLATOR::Oscillator oscillator;
  Whole shape = SAW;
  Float offset = 0;
  Float level = 0;
};

struct Contour {
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = 1,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::LOW,
    .legato = true};
  Source sources[OSCILLATORS];
  CORE::NOISE::Pink noise;
  CORE::FILTER::Ladder ladder;
  Contour loudness;
  Contour filter;
  CORE::SHAPER::Blocker blocker;
  Float tune = 0;
  Float tracking = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sing(Source &source, Float pitch, Whole rate) -> Float;

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::LADDER
