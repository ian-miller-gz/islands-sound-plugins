// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "vocoder.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::VOCODER {

constexpr Whole BANDS = 16;
constexpr Whole CARRIERS = CORE::VOICE::VOICES;
constexpr Float LOUDNESS = 4.0f;
constexpr CORE::NOISE::Register SEEDING = 0x2545F491u;

struct Band {
  CORE::FILTER::Biquad analysis;
  CORE::FILTER::Biquad synthesis;
  CORE::DYNAMICS::Detector follower;
};

struct Carrier {
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::NOISE::White white;
  CORE::ENVELOPE::Gate gate;
};

struct Hiss {
  CORE::FILTER::Biquad high;
  CORE::DYNAMICS::Detector air;
  CORE::DYNAMICS::Detector whole;
  CORE::MODULATOR::Smoother weight;
  CORE::NOISE::White white;
};

struct Vocoder {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  Float level = 0;
  Whole wave = SAW;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
  Band bands[BANDS];
  Carrier carriers[CARRIERS];
  CORE::VOICE::Allocator allocator;
  CORE::ENVELOPE::Envelope envelope;
  Hiss hiss;
  CORE::BLOCK::Meter meter;
};

void build(Vocoder &vocoder);
void settle(Vocoder &vocoder);
void apply(Vocoder &vocoder, const AUDIO::PLUGIN::Event &event);
auto play(Vocoder &vocoder) -> Float;
auto vocode(Vocoder &vocoder, Float modulator, Float carrier) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::VOCODER
