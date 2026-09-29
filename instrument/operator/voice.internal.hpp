// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "operator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::OPERATOR {

constexpr Whole VOICES = 16;
constexpr Whole CONTROL = 16;

struct Routing {
  Whole carriers;
  Whole inputs[OPERATORS];
  Whole into;
  Whole from;
};

struct Voice {
  CORE::OSCILLATOR::Operator units[OPERATORS];
  CORE::ENVELOPE::Walk walks[OPERATORS];
  Float outs[OPERATORS] = {};
  Flag stale = true;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::OSCILLATOR::Table table;
  CORE::ENVELOPE::Segments segments[OPERATORS];
  CORE::MODULATOR::Lfo lfo;
  const Routing *routing = nullptr;
  Float factors[OPERATORS] = {};
  Float levels[OPERATORS] = {};
  Float feedback = 0;
  Float sign = 1;
  Float tune = 0;
  Float vibrato = 0;
  Whole clock = 0;
  CORE::BLOCK::Meter meter;
};

auto bit(Whole unit) -> Whole;
auto routed(Float algorithm) -> const Routing &;

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::OPERATOR
