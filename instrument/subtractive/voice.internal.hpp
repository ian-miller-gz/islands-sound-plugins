// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "subtractive.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::SUBTRACTIVE {

constexpr Float REFERENCE = 60;

enum Curve : Whole { LOUDNESS, CONTOUR, CURVES };

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillators[OSCILLATORS];
  CORE::FILTER::Variable variable;
  CORE::ENVELOPE::Gate gates[CURVES];
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES,
    .mode = CORE::VOICE::POLY,
    .steal = CORE::VOICE::OLDEST,
    .priority = CORE::VOICE::LAST,
    .legato = true};
  Voice voices[VOICES];
  Whole shape = CORE::OSCILLATOR::SAW;
  Float detune = 1;
  Whole kind = CORE::FILTER::LOW;
  CORE::ENVELOPE::Envelope envelopes[CURVES];
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
auto mode(const Synth &synth) -> Whole;

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SUBTRACTIVE
