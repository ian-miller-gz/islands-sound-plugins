// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "tine.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::TINE {

constexpr Whole VOICES = 16;

struct Voice {
  CORE::OSCILLATOR::Operator tine;
  CORE::OSCILLATOR::Operator bar;
  CORE::OSCILLATOR::Operator bell;
  CORE::ENVELOPE::Envelope loudness;
  CORE::ENVELOPE::Gate door;
  CORE::ENVELOPE::Gate strike;
  CORE::ENVELOPE::Gate ring;
  CORE::SHAPER::Blocker blocker;
  Float chime = 0;
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

auto pick(const Synth &synth, Float displacement) -> Float;
auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::TINE
