// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "polymod.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::POLYMOD {

constexpr Whole VOICES = 5;
constexpr Float REFERENCE = 60;

struct Voice {
  CORE::OSCILLATOR::Oscillator master;
  CORE::OSCILLATOR::Sync saw;
  CORE::OSCILLATOR::Sync pulse;
  CORE::FILTER::Ladder ladder;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate loudness;
  CORE::SHAPER::Blocker blocker;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::MODULATOR::Lfo lfo;
  CORE::NOISE::White noise;
  CORE::ENVELOPE::Envelope contour;
  CORE::ENVELOPE::Envelope loudness;
  Float tune = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto modulate(Synth &synth, Voice &voice, Float pitch) -> Float;
auto sing(Synth &synth, Voice &voice, Float pitch, Float mod) -> Float;
auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::POLYMOD
