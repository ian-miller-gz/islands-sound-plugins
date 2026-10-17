// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "tine.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/stereo/stereo.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::TINE {

constexpr Whole VOICES = 16;

struct Voice {
  CORE::OSCILLATOR::Operator tine;
  CORE::OSCILLATOR::Operator bar;
  CORE::OSCILLATOR::Operator bell;
  CORE::ENVELOPE::Envelope loudness;
  CORE::ENVELOPE::Gate door;
  CORE::ENVELOPE::Gate strike;
  CORE::ENVELOPE::Gate ring;
  CORE::FILTER::Pole low;
  CORE::NOISE::Burst hammer;
  CORE::NOISE::Burst damper;
  CORE::SHAPER::Blocker blocker;
  CORE::STEREO::Pair sides;
  Float chime = 0;
  Float velocity = 0;
  Float taper = 1;
  Float hertz = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::OSCILLATOR::Table sine;
  CORE::ENVELOPE::Envelope strike;
  CORE::ENVELOPE::Envelope ring;
  CORE::MODULATOR::Lfo tremolo;
  Float drive = 1;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
void tune(Synth &synth, Whole at);
void shape(Synth &synth, Whole at);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto curve(const Synth &synth, Float velocity) -> Float;
void touch(Synth &synth, Whole at);
void seed(Synth &synth);

auto pick(const Synth &synth, Float displacement) -> Float;
auto sound(Synth &synth) -> CORE::STEREO::Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::TINE
