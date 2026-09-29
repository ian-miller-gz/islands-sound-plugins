// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "mono.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::MONO {

enum Slide : Whole { STILL, AUTOMATIC, GLIDING };
enum Wave : Whole { TRIANGLE, SQUARE, RANDOM, NOISY };
enum Source : Whole { CYCLE, MANUAL, CONTOUR };
enum Trigger : Whole { CYCLED, GATED, TRIGGERED };
enum Amplifier : Whole { SHAPED, OPEN };

constexpr Float REFERENCE = 60;

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
    .priority = CORE::VOICE::LAST,
    .legato = false};
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Oscillator sub;
  CORE::NOISE::White noise;
  CORE::MODULATOR::Lfo lfo;
  CORE::FILTER::Ladder ladder;
  Contour envelope;
  Contour gate;
  CORE::SHAPER::Blocker blocker;
  Float offset = 0;
  Float divisor = 1;
  Float narrow = CORE::OSCILLATOR::SQUARE;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void strike(Contour &contour, Float velocity);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::MONO
