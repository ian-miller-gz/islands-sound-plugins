// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "vowel.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/stereo/stereo.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::VOWEL {

constexpr Whole STRIDE = 16;
constexpr Float SPAN = 400.0f;
constexpr Float CENT = 100.0f;
constexpr CORE::NOISE::Register SCATTER = 0x2545F491u;

namespace WOBBLE {
constexpr Float TIME = 0.4f;
}  // namespace WOBBLE

namespace JITTER {
constexpr Float TIME = 0.01f;
constexpr Float SHARE = 5.0f;
}  // namespace JITTER

namespace TRACT {
constexpr Float TIME = 0.02f;
}  // namespace TRACT

struct Throat {
  CORE::GLOTTIS::Source source;
  CORE::FILTER::Formant formant;
  CORE::MODULATOR::Lfo vibrato;
  CORE::MODULATOR::Drift wobble;
  CORE::MODULATOR::Drift jitter;
  CORE::ENVELOPE::Gate gate;
  Float place = 0;
  CORE::STEREO::Pair pan;
};

struct Singer {
  Whole rate = 0;
  Whole channels = 0;
  Whole pace = 1;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  Float shift = 1;
  Float pole = 1;
  Float sway = 0;
  CORE::FILTER::Tract tract;
  CORE::BLOCK::Stride stride;
  CORE::MODULATOR::Lfo motion;
  CORE::OSCILLATOR::Table table;
  CORE::ENVELOPE::Envelope envelope;
  CORE::VOICE::Allocator allocator;
  Throat throats[THROATS];
  CORE::BLOCK::Meter meter;
};

void seat(Singer &singer, Whole at);
void settle(Singer &singer);
void allot(Singer &singer);
void release(Singer &singer);

void apply(Singer &singer, const AUDIO::PLUGIN::Event &event);

void aim(Singer &singer);
void steer(Singer &singer);
void steer(Singer &singer, Whole at, Flag struck);
auto sing(Singer &singer) -> CORE::STEREO::Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::VOWEL
