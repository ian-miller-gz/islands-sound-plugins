// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chip.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/phase/phase.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::CHIP {

constexpr Float CPU = 1789773;
constexpr Whole VOICES = 1;
constexpr Whole PULSES = 2;
constexpr Whole ENVELOPES = 3;
constexpr Whole NOISE = 2;
constexpr Whole FILTERS = 3;
constexpr Whole POSITIONS = 3;

struct Clock {
  CORE::PHASE::Wheel phase = 0;
  CORE::PHASE::Wheel step = 0;
};

struct Shape {
  Whole level = 0;
  Whole decay = 0;
  Whole sustain = 0;
};

struct Steps {
  Whole level = 0;
  Whole count = 0;
  Flag held = false;
};

struct Pulse {
  Clock clock;
  Whole duty = 0;
  Flag muted = true;
};

struct Wave {
  Clock clock;
  Flag muted = true;
};

struct Lfsr {
  Whole bits = 1;
  Whole tap = 1;
  Float clocks = 0;
  Float debt = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::LAST,
    .legato = false};
  Pulse pulses[PULSES];
  Wave triangle;
  Lfsr noise;
  Shape shapes[ENVELOPES];
  Steps steps[ENVELOPES];
  Clock frame;
  Clock arpeggio;
  Whole position = 0;
  Flag gated = false;
  Flag stale = true;
  Float tune = 0;
  CORE::FILTER::Pole filters[FILTERS];
  CORE::BLOCK::Meter meter;
};

auto tick(Clock &clock) -> Flag;
void strike(Synth &synth);
void lift(Synth &synth);
void clock(Synth &synth);
auto shift(Lfsr &noise) -> Whole;
auto mixed(Whole pulses, Whole triangle, Whole noise) -> Float;

void settle(Synth &synth);
void retune(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CHIP
