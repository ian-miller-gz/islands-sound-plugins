// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "wavetable.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::WAVETABLE {

constexpr Whole VOICES = 8;
constexpr Whole SOURCES = 2;
constexpr Whole CONTROL = 16;
constexpr Whole PARTIALS = 32;
constexpr Whole KEYS = 8;
constexpr Whole SPAN = (FRAMES - 1) / (KEYS - 1);
constexpr Float REFERENCE = 60;
static_assert(SPAN * (KEYS - 1) == FRAMES - 1);

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillators[SOURCES];
  CORE::FILTER::Ladder ladder;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  Flag stale = true;
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
  Whole clock = 0;
  CORE::BLOCK::Meter meter;
};

using Spectrum = auto (*)(Float place, Float harmonic) -> Float;

void build(CORE::OSCILLATOR::Table &table, Whole layout);

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::WAVETABLE

namespace SOUND::WAVETABLE::SPECTRUM {
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
}  // namespace SOUND::WAVETABLE::SPECTRUM
