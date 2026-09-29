// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include "ensemble.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::ENSEMBLE {

using Counter = std::uint64_t;

constexpr Whole LINES = 3;
constexpr Whole CLASSES = 12;
constexpr Whole TOP = 120;
constexpr Whole REGISTERS = 4;
constexpr Float BRIGHT = 7000;
constexpr Float MELLOW = 2500;
constexpr Float WARM = 1600;
constexpr Float DEEP = 600;
constexpr Float BLEND = 0.6f;

struct Register {
  Float eight;
  Float sixteen;
  Float cutoff;
};

inline constexpr Register PANEL[] = {
  {1, 0, BRIGHT}, {1, 0, MELLOW}, {BLEND, 1, WARM}, {0, 1, DEEP}};
static_assert(std::size(PANEL) == REGISTERS);

struct Key {
  Flag held = false;
  Float target = 0;
  Float level = 0;
};

struct Sums {
  Float eight = 0;
  Float sixteen = 0;
};

struct Pair {
  Float left = 0;
  Float right = 0;
  Float whole = 0;
};

struct Ensemble {
  CORE::FILTER::Pole bucket;
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[LINES];
  CORE::MODULATOR::Lfo slow[LINES];
  CORE::MODULATOR::Lfo fast[LINES];
  Float wet = 0;
  Float chorus = 0;
  Float vibrato = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Key keys[CORE::PHASE::PITCHES];
  Counter counters[CLASSES] = {};
  Counter steps[CLASSES] = {};
  Whole held = 0;
  Float ramp = 1;
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
  CORE::FILTER::Biquad tones[REGISTERS];
  Ensemble ensemble;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto divide(Synth &synth) -> Sums;

void build(Ensemble &ensemble, Whole rate);
void settle(Ensemble &ensemble, const Float *rows, Whole rate);
auto tick(Ensemble &ensemble, Float in) -> Pair;

auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ENSEMBLE
