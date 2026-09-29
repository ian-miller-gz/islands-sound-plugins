// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "whisper.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::WHISPER {

constexpr Whole VOICES = 8;
constexpr Float LOUDNESS = 8.0f;
constexpr Float CENTRE = 60.0f;
constexpr Float CENT = 100.0f;
constexpr CORE::NOISE::Register SCATTER = 0x2545F491u;

struct Hiss {
  CORE::NOISE::White white;
  CORE::NOISE::Pink pink;
  CORE::NOISE::Brown brown;
  CORE::NOISE::Blue blue;
  CORE::FILTER::Formant formant;
  CORE::ENVELOPE::Gate gate;
};

struct Mouth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  Whole colour = WHITE;
  CORE::ENVELOPE::Envelope envelope;
  CORE::VOICE::Allocator allocator;
  Hiss hisses[VOICES];
  CORE::BLOCK::Meter meter;
};

void seed(Hiss &hiss, CORE::NOISE::Register state);

void shape(Mouth &mouth, Whole at);

void settle(Mouth &mouth);

void apply(Mouth &mouth, const AUDIO::PLUGIN::Event &event);

auto breathe(Mouth &mouth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::WHISPER
