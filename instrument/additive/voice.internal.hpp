// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "additive.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/phase/phase.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::ADDITIVE {

constexpr Whole VOICES = 8;
constexpr Whole STRIDE = 16;
constexpr Float REFERENCE = 60;

struct Bank {
  CORE::PHASE::Wheel phases[BANK] = {};
  CORE::PHASE::Wheel steps[BANK] = {};
  Float gains[BANK] = {};
  Float slopes[BANK] = {};
};

struct Voice {
  Bank bank;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  CORE::BLOCK::Stride stride;
  Float key = 0;
  Float sweep = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::OSCILLATOR::Table table;
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::ENVELOPE::Envelope contour;
  CORE::ENVELOPE::Envelope door;
  CORE::MODULATOR::Lfo lfo;
  Float weights[BANK] = {};
  Float ratios[BANK] = {};
  Float tune = 0;
  Float swing = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

void shape(Synth &synth, Voice &voice, const CORE::VOICE::Note &note);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ADDITIVE
