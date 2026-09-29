// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "tuner.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/pitch/pitch.hpp"

namespace SOUND::TUNER {

constexpr Float LOWEST = 70.0f;
constexpr Float HIGHEST = 1000.0f;
constexpr Whole HOP = 512;
constexpr Whole STRIDE = 32;
constexpr Float WINDOW = 0.03f;
constexpr Float VOICED = 0.8f;
constexpr Float HUMANE = 8.0f;
constexpr Float CENT = 100.0f;
constexpr Integer NONE = -1;

struct Shifter {
  CORE::PITCH::Grains grains;
  CORE::PITCH::Formant formant;
};

struct Tuner {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PITCH::Detector detector;
  CORE::MODULATOR::Smoother correction;
  Vector<Shifter> shifters;
  Flag keep = true;
  Integer note = NONE;
  Float target = 0;
  Whole count = 0;
  CORE::BLOCK::Meter meter;
};

void build(Tuner &tuner);
void settle(Tuner &tuner);
void retime(Tuner &tuner, Flag held);
void apply(Tuner &tuner, const AUDIO::PLUGIN::Event &event);
void follow(Tuner &tuner, Float sample);
void steer(Tuner &tuner);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::TUNER
