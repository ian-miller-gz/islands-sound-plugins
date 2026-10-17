// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include "ensemble.hpp"
#include "voice.ensemble.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::ENSEMBLE {

using Counter = std::uint64_t;

constexpr Whole CLASSES = 12;
constexpr Whole TOP = 120;
constexpr Whole REGISTERS = 4;
constexpr Whole BANDS = 3;
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
  Float velocity = CORE::ENVELOPE::FULL;
  CORE::ENVELOPE::Gate gate;
};

struct Resonator {
  CORE::FILTER::Biquad bands[BANDS];
  Float wet = 0;
};

struct Sums {
  Float eight = 0;
  Float sixteen = 0;
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
  Resonator resonator;
  Ensemble ensemble;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto divide(Synth &synth, Float swell) -> Sums;

void settle(Resonator &resonator, Float formant, Whole rate);
auto tick(Resonator &resonator, Float in) -> Float;

auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ENSEMBLE
