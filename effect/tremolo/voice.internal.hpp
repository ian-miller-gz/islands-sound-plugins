// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "tremolo.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::TREMOLO {

constexpr Float SMOOTHING = 0.002f;
constexpr Float DEGREES = 360.0f;
constexpr Whole SIDES = 2;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

inline constexpr CORE::MODULATOR::Wave WAVES[] = {
  CORE::MODULATOR::SINE, CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SQUARE};

struct Channel {
  CORE::MODULATOR::Lfo lfo;
  CORE::MODULATOR::Smoother smoother;
};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Channel> strips;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void settle(Effect &effect);
void place(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::TREMOLO
