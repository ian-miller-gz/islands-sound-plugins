// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "dco.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::DCO {

constexpr Whole VOICES = 6;
constexpr Float REFERENCE = 60;

struct Contour {
  CORE::ENVELOPE::Envelope shape;
  CORE::ENVELOPE::Gate gate;
};

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Oscillator sub;
  CORE::FILTER::Transistor transistor;
  CORE::SHAPER::Oversampler oversampler;
  CORE::BLOCK::Stride stride;
  Contour filter;
  Contour amplifier;
  CORE::SHAPER::Blocker blocker;
  CORE::MODULATOR::Drift drift;
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
  CORE::VOICE::Trim trims[VOICES];
  CORE::MODULATOR::Lfo lfo;
  CORE::NOISE::White noise;
  CORE::FILTER::Biquad highpass;
  CORE::LINE::Chorus chorus;
  Float tune = 0;
  Float level = 1;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);
void assign(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

void settle(CORE::FILTER::Biquad &highpass, Whole position, Whole rate);
void settle(CORE::LINE::Chorus &chorus, Whole mode, Whole rate);

auto sing(Synth &synth, Voice &voice, Float pitch, Float shape) -> Float;
auto filter(Synth &synth, Whole at, Float in, Float contour, Float lfo)
  -> Float;
auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::DCO
