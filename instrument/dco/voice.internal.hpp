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

namespace SOUND::DCO {

constexpr Whole VOICES = 6;
constexpr Whole LINES = 3;
constexpr Float REFERENCE = 60;

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Oscillator sub;
  CORE::FILTER::Ladder ladder;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  CORE::SHAPER::Blocker blocker;
};

struct Chorus {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[LINES];
  CORE::MODULATOR::Lfo lfos[LINES];
  Float wet = 0;
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
  CORE::MODULATOR::Lfo lfo;
  CORE::NOISE::White noise;
  CORE::ENVELOPE::Envelope contour;
  CORE::ENVELOPE::Envelope door;
  CORE::FILTER::Biquad highpass;
  Chorus chorus;
  Float tune = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

void build(Chorus &chorus, Whole rate);
void settle(Chorus &chorus, Whole mode, Whole rate);
auto tick(Chorus &chorus, Float in) -> Pair;

auto sing(Synth &synth, Voice &voice, Float pitch, Float shape) -> Float;
auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::DCO
