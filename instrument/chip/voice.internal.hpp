// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chip.hpp"
#include "voice.channels.hpp"
#include "../../core/block/block.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
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
constexpr Whole QUARTER = 240;

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
  CORE::MODULATOR::Lfo vibrato;
  Float wobble = 0;
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
