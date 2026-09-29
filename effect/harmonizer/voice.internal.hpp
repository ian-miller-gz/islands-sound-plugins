// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "harmonizer.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/pitch/pitch.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::HARMONIZER {

constexpr Float LOWEST = 70.0f;
constexpr Float TOPMOST = 1000.0f;
constexpr Whole HOP = 512;
constexpr Whole STRIDE = 32;
constexpr Float WINDOW = 0.03f;
constexpr Float VOICED = 0.8f;
constexpr Float FADE = 0.01f;
constexpr Float QUIET = 0.0001f;
constexpr Float CENT = 100.0f;
constexpr Float RANGE = 2400.0f;

struct Part {
  CORE::PITCH::Grains grains;
  CORE::PITCH::Formant formant;
  CORE::MODULATOR::Smoother gate;
  CORE::MODULATOR::Smoother glide;
  Flag fresh = false;
  Float left = 0;
  Float right = 0;
};

struct Harmonizer {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PITCH::Detector detector;
  CORE::VOICE::Allocator allocator;
  Part parts[VOICES];
  Float pitch = MIDDLE;
  Flag keep = true;
  Whole count = 0;
  CORE::BLOCK::Meter meter;
};

void build(Harmonizer &harmonizer);
void settle(Harmonizer &harmonizer);
void apply(Harmonizer &harmonizer, const AUDIO::PLUGIN::Event &event);
void follow(Harmonizer &harmonizer, Float sample);
void steer(Harmonizer &harmonizer);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::HARMONIZER
