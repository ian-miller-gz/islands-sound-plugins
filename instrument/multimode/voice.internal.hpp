// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "multimode.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::MULTIMODE {

constexpr Float REFERENCE = 60;
constexpr Whole OVERSAMPLING = 2;
constexpr Whole STRIDE = 16;

struct Contour {
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
};

struct Voice {
  CORE::OSCILLATOR::Oscillator first;
  CORE::OSCILLATOR::Sync saw;
  CORE::OSCILLATOR::Sync pulse;
  CORE::FILTER::Variable variable;
  CORE::SHAPER::Oversampler oversampler;
  CORE::BLOCK::Stride stride;
  CORE::MODULATOR::Drift drift;
  Float wander = 0;
  Contour contour;
  Contour loudness;
  CORE::SHAPER::Blocker blocker;
};

struct Pair {
  Float left = 0;
  Float right = 0;
  Float whole = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::VOICE::Trim trims[VOICES];
  CORE::MODULATOR::Lfo lfo;
  Float tune = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
void seed(Synth &synth);
void wander(Synth &synth);
void drift(Synth &synth, Voice &voice);
auto stray(const Synth &synth, Whole at) -> Float;
auto open(const Synth &synth, Whole at) -> Float;
auto stretch(const Synth &synth, Whole at) -> Float;

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sing(Synth &synth, Voice &voice, Float pitch, Float width) -> Float;
void steer(Synth &synth, Voice &voice, Float cutoff);
auto filter(Synth &synth, Voice &voice, Float in) -> Float;
auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::MULTIMODE
