// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "chip.hpp"
#include "voice.channels.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
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
constexpr Whole SWEEPING = 120;
constexpr Whole FLOOR = 8;
constexpr Whole CEILING = 2047;
static_assert(CHANNEL::WAVE == PULSES);

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
  Timer timers[CHANNEL::COUNT];
  CORE::OSCILLATOR::Edges edges;
  Shape shapes[ENVELOPES];
  Steps steps[ENVELOPES];
  Float weight = 1;
  Clock frame;
  Whole quarters = 0;
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
void advance(Pulse &pulse);
void advance(Wave &triangle);
auto level(const Synth &synth) -> Float;
auto run(Synth &synth, Float before) -> Float;

auto target(const Synth &synth, Whole at) -> Whole;
auto silenced(const Synth &synth, Whole at) -> Flag;
void arm(Synth &synth);
void sweep(Synth &synth);

void settle(Synth &synth);
void retune(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CHIP
