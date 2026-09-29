// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "vowel.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::VOWEL {

constexpr Whole VOICES = 8;
constexpr Float SPAN = 400.0f;
constexpr Float CENT = 100.0f;
constexpr CORE::NOISE::Register SCATTER = 0x2545F491u;

struct Throat {
  CORE::GLOTTIS::Source source;
  CORE::FILTER::Formant formant;
  CORE::MODULATOR::Lfo vibrato;
  CORE::ENVELOPE::Gate gate;
};

struct Singer {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  CORE::OSCILLATOR::Table table;
  CORE::ENVELOPE::Envelope envelope;
  CORE::VOICE::Allocator allocator;
  Throat throats[VOICES];
  CORE::BLOCK::Meter meter;
};

void settle(Singer &singer);

void apply(Singer &singer, const AUDIO::PLUGIN::Event &event);

auto sing(Singer &singer) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::VOWEL
