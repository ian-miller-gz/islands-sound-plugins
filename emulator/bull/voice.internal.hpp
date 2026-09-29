// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "bull.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::BULL {

enum Voice : Whole { BASS, HORN, DEEP, MANUAL };
enum Range : Whole { LOW, FULL };

constexpr Whole FIRST = 0;
constexpr Whole SECOND = 1;

struct Contour {
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float chain[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = 1,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::LOW,
    .legato = true};
  CORE::OSCILLATOR::Oscillator oscillators[OSCILLATORS];
  CORE::OSCILLATOR::Oscillator sub;
  Whole waves[OSCILLATORS] = {};
  CORE::FILTER::Ladder ladder;
  Contour loudness;
  Contour filter;
  CORE::SHAPER::Blocker blocker;
  Float offset = 0;
  Float beat = 1;
  CORE::BLOCK::Meter meter;
};

void compose(Synth &synth);

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::BULL
