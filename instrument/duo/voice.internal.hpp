// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "duo.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::DUO {

constexpr Float REFERENCE = 60;

struct Contour {
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
};

struct Tones {
  Float mixed = 0;
  Float saw = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator low = {
    .count = 1,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::LOW,
    .legato = true};
  CORE::VOICE::Allocator high = {
    .count = 1,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::HIGH,
    .legato = true};
  CORE::OSCILLATOR::Oscillator first;
  CORE::OSCILLATOR::Sync second;
  CORE::NOISE::Pink noise;
  CORE::MODULATOR::Lfo lfo;
  CORE::MODULATOR::Smoother lag;
  Float input = 0;
  Float held = 0;
  CORE::FILTER::Pole highpass;
  CORE::FILTER::Variable variable;
  CORE::FILTER::Ladder ladder;
  Contour adsr;
  Contour ar;
  CORE::SHAPER::Blocker blocker;
  Float tune = 0;
  Float fine = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void strike(Synth &synth, Float velocity);
void sample(Synth &synth);
void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sing(Synth &synth, Float first, Float second, Float width) -> Tones;
auto filter(
  Synth &synth, Float in, Float key, Float lfo, Float random,
  Float contour) -> Float;
auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::DUO
