// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "supersaw.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::SUPERSAW {

constexpr Whole VOICES = 8;
constexpr Whole CONTROL = 16;
constexpr Float REFERENCE = 60;

struct Voice {
  CORE::OSCILLATOR::Super super;
  CORE::FILTER::Biquad highpass;
  CORE::FILTER::Ladder ladder;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  Flag stale = true;
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
  CORE::ENVELOPE::Envelope contour;
  CORE::ENVELOPE::Envelope door;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
  Float tune = 0;
  Whole clock = 0;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::SUPERSAW
