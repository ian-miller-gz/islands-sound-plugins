// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "wavetable.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/stereo/stereo.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::WAVETABLE {

constexpr Whole VOICES = CORE::VOICE::VOICES;
constexpr Whole SOURCES = 2;
constexpr Whole STRIDE = 16;
constexpr Whole PARTIALS = CORE::OSCILLATOR::TOP;
constexpr Whole KEYS = 8;
constexpr Whole SPAN = (FRAMES - 1) / (KEYS - 1);
constexpr Float REFERENCE = 60;
static_assert(SPAN * (KEYS - 1) == FRAMES - 1);

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillators[SOURCES];
  Whole levels[SOURCES] = {};
  CORE::FILTER::Ladder ladder;
  CORE::FILTER::Variable variable;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  CORE::BLOCK::Stride stride;
  CORE::STEREO::Pair sides;
};

struct Sum {
  CORE::STEREO::Pair sides;
  Float whole = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::OSCILLATOR::Table tables[TABLES];
  CORE::VOICE::Allocator allocator = {
    .count = VOICES, .mode = CORE::VOICE::POLY, .steal = CORE::VOICE::OLDEST};
  Voice voices[VOICES];
  CORE::ENVELOPE::Envelope contour;
  CORE::ENVELOPE::Envelope door;
  CORE::MODULATOR::Lfo lfo;
  Float tune = 0;
  Float weight = 1;
  Whole warp = 0;
  Float slope = 1;
  CORE::BLOCK::Meter meter;
};

using Spectrum = auto (*)(Float place, Float harmonic) -> Float;

void build(CORE::OSCILLATOR::Table &table, Whole layout);

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

void settle(
  const Synth &synth, Voice &voice, const CORE::VOICE::Note &note, Float key,
  Float contour);
auto filter(const Synth &synth, Voice &voice, Float in) -> Float;

auto slope(Whole kind, Float amount) -> Float;
auto warp(const Synth &synth, CORE::PHASE::Wheel phase) -> CORE::PHASE::Wheel;

auto sound(Synth &synth) -> Sum;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::WAVETABLE

namespace SOUND::PLUGINS::WAVETABLE::SPECTRUM {
auto sine(Float place, Float harmonic) -> Float;
auto harmonic(Float place, Float harmonic) -> Float;
auto saw(Float place, Float harmonic) -> Float;
auto square(Float place, Float harmonic) -> Float;
auto hollow(Float place, Float harmonic) -> Float;
auto triangle(Float place, Float harmonic) -> Float;
auto pulse(Float place, Float harmonic) -> Float;
auto organ(Float place, Float harmonic) -> Float;
auto octaves(Float place, Float harmonic) -> Float;
auto formant(Float place, Float harmonic) -> Float;
auto vowel(Float place, Float harmonic) -> Float;
auto comb(Float place, Float harmonic) -> Float;
auto sync(Float place, Float harmonic) -> Float;
auto prime(Float place, Float harmonic) -> Float;
auto scatter(Float place, Float harmonic) -> Float;
auto resonant(Float place, Float harmonic) -> Float;
}  // namespace SOUND::PLUGINS::WAVETABLE::SPECTRUM
