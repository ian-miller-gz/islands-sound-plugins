// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "whisper.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::WHISPER {

constexpr Whole HISSES = 8;
constexpr Whole STRIDE = 16;
constexpr Float LOUDNESS = 8.0f;
constexpr Float CENTRE = 60.0f;
constexpr Float CENT = 100.0f;
constexpr Float SPAN = 400.0f;
constexpr Float GLIDING = 0.02f;
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
  Float part = 0;
  Float pole = 0;
  Float sway = 0;
  CORE::BLOCK::Stride stride;
  CORE::MODULATOR::Lfo motion;
  CORE::ENVELOPE::Envelope envelope;
  CORE::VOICE::Allocator allocator;
  Hiss hisses[HISSES];
  CORE::BLOCK::Meter meter;
};

void seed(Hiss &hiss, CORE::NOISE::Register state);

void place(Mouth &mouth);

void shape(Mouth &mouth, Whole at, Flag leap);

void steer(Mouth &mouth);

void settle(Mouth &mouth);

void apply(Mouth &mouth, const AUDIO::PLUGIN::Event &event);

auto breathe(Mouth &mouth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::WHISPER
